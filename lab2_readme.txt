lab2
Ke Peng k.peng@wustl.edu
Zichao Liang liang.zichao@wustl.edu
Siqian Hou h.siqian@wustl.edu

1. Bug fix: Using custom operator< as the comparator of std::sort algorithm to sort the HoldEmHandRank vector causes a bug. The error was caused by the reference type member "std::string& playerName" in the struct. Having a non-static data member of reference type will cause the default copy assignment operator of the struct to be defined as "deleted". So we need either add a copy constructor manually or remove the "&" of the data member. After removing the "&", the bug is fixed.

2. Trial results:

Trial 1
$ ./lab2 HoldEm player1        
Usage: ./lab2 Pinochle/HoldEm player1 player2 ...
[Pinochle]: must have 4 players
[HoldEm]: between 2 and 9 players

- We can see that the usage information is correctly printed.

Trial 2
$ ./lab2 Pinochle player1 player2 player3 player4
player1's hand:
KH JS AH 
AS QD 10D 
10H JS 10C 
JH AC AD 
Melds:
Hundred Aces 100

player2's hand:
9H QC KS 
KD JD KS 
QH KD JC 
9D 9S QH 
Melds:

player3's hand:
10S AS KC 
10C KC JH 
AH QC 9C 
KH AD 10S 
Melds:

player4's hand:
QD QS QS 
JD 10H 9S 
9H JC 10D 
9D 9C AC 
Melds:
Pinochle 40

End the game? (yes/no): yes

- We can see the melds are printed correctly.

Trial 3
$ ./lab2 HoldEm player1 player2 player3               
player1's hand:
9D 6H 
player2's hand:
8H AH 
player3's hand:
JC JD 
BOARD(flop): 
8D JS 4S 

player3's rank: Three of a Kind
player2's rank: Pair
player1's rank: X-High

BOARD(turn): 
8D JS 4S 
3S 
BOARD(river): 
8D JS 4S 
3S 7S 
End the game? (yes/no): yes

- We can see the hand rank are sorted and printed correctly.

Trial 4
$ ./lab2 HoldEm player1 player2 player3
player1's hand:
QH 8H 
player2's hand:
2H JD 
player3's hand:
QC AD 
BOARD(flop): 
3S 9H KC 

player3's rank: X-High
player1's rank: X-High
player2's rank: X-High

BOARD(turn): 
3S 9H KC 
10H 
BOARD(river): 
3S 9H KC 
10H KD 
End the game? (yes/no): yes

- We can see that the initial ordering of cards in each game's deck differs from run to run of the program,
and the subsequent ordering of cards in the game's deck differs from turn to turn of play within each run of the program.