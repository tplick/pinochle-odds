// pin.c
// 21 Dec 2007
// Pinochle calculations.
// updated 27 Oct 2011 to count unique hands, no aces
// Copyright (C) Tom Plick 2007-2011.

/*
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include <stdio.h>
#include <stdlib.h>

#define MIN(x, y)  ((x) < (y) ? (x) : (y))

int multContrib[3] = {0, 1, 0};

long long int SUCCESSES = 0, ALL_TRIALS = 0;

typedef int (*FN)(int *);
struct Test {
    char name[50];
    FN func;
    long long count;
};

struct Test TESTS[];


void printHand(int * a){
    int i;
    for (i = 0; i < 24; i++)
        printf("%d ", a[i]);
    printf("\n");
}

void doTest(int * a, int mult){
    int i;
    for (i = 0; TESTS[i].name[0]; i++){
        if (TESTS[i].func(a))
            TESTS[i].count += (1 << mult);
    }
}

int matchAnyHand(int * a){ return 1; }

int doesContainStraight(int * a){
    return (a[0] && a[1] && a[2] && a[3] && a[4]) ||
           (a[6] && a[7] && a[8] && a[9] && a[10]) ||
           (a[12] && a[13] && a[14] && a[15] && a[16]) ||
           (a[18] && a[19] && a[20] && a[21] && a[22]);
}

int doesContainDoubleStraight(int * a){
    return (a[0] == 2 && a[1] == 2 && a[2] == 2 && a[3] == 2 && a[4] == 2) ||
           (a[6] == 2 && a[7] == 2 && a[8] == 2 && a[9] == 2 && a[10] == 2) ||
           (a[12] == 2 && a[13] == 2 && a[14] == 2 && a[15] == 2 && a[16] == 2) ||
           (a[18] == 2 && a[19] == 2 && a[20] == 2 && a[21] == 2 && a[22] == 2);
}

int hasTwoDiffStraights(int * a){
    return (a[0] && a[1] && a[2] && a[3] && a[4]) +
           (a[6] && a[7] && a[8] && a[9] && a[10]) +
           (a[12] && a[13] && a[14] && a[15] && a[16]) +
           (a[18] && a[19] && a[20] && a[21] && a[22]) >= 2;
}


int hasFourAces(int * a){
    return a[0] && a[1] && a[2] && a[3];
}

int hasEightAces(int * a){
    return a[0] + a[1] + a[2] + a[3] == 8;
}

int hasSevenAces(int * a){
    return a[0] + a[1] + a[2] + a[3] == 7;
}

int hasNoAces(int * a){
    return a[0] + a[1] + a[2] + a[3] == 0;
}

int containsRoundhouse(int * a){
    return a[0] && a[6] && a[12] && a[18]
        && a[1] && a[7] && a[13] && a[19];
}


int hasPinochle(int * a){
    return a[0] && a[1];
}

int hasDoublePinochle(int * a){
    return a[0] + a[1] == 4;
}

int hasMarriage(int * a)
{
    return (a[0] && a[1]) || (a[2] && a[3])
        || (a[4] && a[5]) || (a[6] && a[7]);
}

int hasNoMeld(int * a)
{
    // look for marriage
    int suit;
    for (suit = 0; suit < 24; suit += 6){
        if (a[suit+2] && a[suit+3])
            return 0;
    }

    // look for four of a kind
    int rank;
    for (rank = 0; rank < 5; rank++){
        if (rank == 1) rank++;

        if (a[rank] && a[rank+6] && a[rank+12] && a[rank+18])
            return 0;
    }

    // pinochle
    if (a[3] && a[22])
        return 0;

    return 1;
}

long long int Unique = 0;
void combos(int * start, int * end, int left, int mult){
    if (2 * (end - start) < left)
        return;

    if (start == end){
        if (!left){
            doTest(start, mult);
            Unique++;
        }
        return;
    }

    int i;
    end--;
    for (i = 0; i <= 2 && i <= left; i++){
        *end = i;
        combos(start, end, left - i, mult + (i & 1));
    }
}

struct Test TESTS[] = {
    {"count hands",  matchAnyHand,         0},  // do not comment out

    {"marriage?",    hasMarriage,  0},
    {"straight?",    doesContainStraight,  0},
    {"double straight?",    doesContainDoubleStraight,  0},
    {"has two straights in different suits?", hasTwoDiffStraights, 0},

    {"4 aces",       hasFourAces,          0},
    {"7 aces",       hasSevenAces,         0},
    {"8 aces",       hasEightAces,         0},
    {"no aces",      hasNoAces,            0},

    {"pinochle?",    hasPinochle,          0},
    {"double pinochle?", hasDoublePinochle, 0},

    {"roundhouse?",  containsRoundhouse,   0},
    {"has no meld?", hasNoMeld, 0},
    {"", matchAnyHand, 0}  // do not comment out
};


int main(int argc, char *argv[]){
    if (argc < 2){
        printf("Must supply size of hand.\n");
        exit(1);
    }

    int handsize = atoi(argv[1]);
    printf("Dealing %d cards.\n\n", handsize);

    int a[24], i;
    combos(a, a + 24, handsize, 0);
    long long ALL_TRIALS = TESTS[0].count;

    printf("Unique hands: %lld out of %lld (%.7f%%)\n\n", Unique, ALL_TRIALS, 
            100 * Unique / (double) ALL_TRIALS);

    for (i = 0; TESTS[i].name[0]; i++){
        printf("Test: %s\nProbability: %.7f%% (%lld out of %lld)\n\n",
               TESTS[i].name,
               100 * TESTS[i].count/(double)ALL_TRIALS,
               TESTS[i].count, ALL_TRIALS);
    }

    return 0;
}
