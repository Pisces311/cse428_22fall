lab2
Ke Peng k.peng@wustl.edu
Zichao Liang liang.zichao@wustl.edu
Siqian Hou h.siqian@wustl.edu

1. Bug fix: Using custom operator< as the comparator of std::sort algorithm to sort the HoldEmHandRank vector causes a bug. The error was caused by the reference type member "std::string& playerName" in the struct. Having a non-static data member of reference type will cause the default copy assignment operator of the struct to be defined as "deleted". So we need either add a copy constructor manually or remove the "&" of the data member. After removing the "&", the bug is fixed.

2. Trial results:

Trial 1
$ ./lab1 HoldEm player1        
Usage: ./lab1 Pinochle/HoldEm player1 player2 ...
[Pinochle]: must have 4 players
[HoldEm]: between 2 and 9 players

We can see that the usage information is correctly printed.

Trial 2
$ ./lab1 Pinochle player1 player2 player3 player4
player1's hand:
AS QD KD 
AD KC JD 
9S AS AD 
9D AC 9C 
player2's hand:
JC KS JS 
KH QH 10D 
10H QC QS 
QH 9S AH 
player3's hand:
10C 9D 10C 
KD 9H JH 
10H KC 10D 
JH 9H AH 
player4's hand:
QD JD KH 
QC JC 9C 
AC 10S 10S 
KS JS QS 
End the game? (yes/no): yes

Trial 3
$ ./lab1 HoldEm player1 player2                  
player1's hand:
8H 9C 
player2's hand:
8S 6C 
BOARD(flop): 
7D 9D KD 
BOARD(turn): 
7D 9D KD 
8D 
BOARD(river): 
7D 9D KD 
8D 10C 
End the game? (yes/no): yes

Trial 4
$ ./lab1 HoldEm player1 player2
player1's hand:
5C 7C 
player2's hand:
3S 10C 
BOARD(flop): 
6D QH 4H 
BOARD(turn): 
6D QH 4H 
JH 
BOARD(river): 
6D QH 4H 
JH 9C 
End the game? (yes/no): yes

We can see that the initial ordering of cards in each game's deck differs from run to run of the program,
and the subsequent ordering of cards in the game's deck differs from turn to turn of play within each run of the program.