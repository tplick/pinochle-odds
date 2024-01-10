Back in 2011, I was curious about the odds of being dealt particular sets of
cards in pinochle, a popular card game.  Some of the probabilities are
difficult to compute mathematically because the pinochle deck contains two of
every card from 9 to ace.  To calculate the odds, I wrote a program in C that
considers every possible hand and takes into account the relative probability
of each hand being dealt.  It uses backtracking to speed things up.

When I wrote the program, it took approximately 5 minutes to run on all the
12-card hands, and it took approximately an hour to run on all the 16-card
hands.  My computer today runs these programs about 20 times faster (taking
around 15 seconds and 3 minutes, respectively).

This repository contains:
    - pin.c, the program that I wrote to calculate the odds
    - pinochle2011.txt, the file containing the output for 12-card and
      16-card hands
    - index.html, the web page explaining the results (and reporting the
      numbers in a nice table)

These files are also on my website at https://www.amecy.com/pinochle/ .

