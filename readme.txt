lab3
Ke Peng k.peng@wustl.edu
Zichao Liang liang.zichao@wustl.edu
Siqian Hou h.siqian@wustl.edu

1. Pinochle Bug fix:
    1) One of the bugs in Pinochle we meet is when a player's highest card has the same rank and suit as the highest card in trick, the player will still play it instead of play a lowest card. The reason is that when we compare the highest hand card with the trick, we originally use > instead of of >=., so when the cards are the same, bug will happen.

    2) Another bug from Pinochle is when the game approach the end, the player may play a card that he actually don't have. Since when we are finding the highest card a player should play, we will set a default value as the lowest card rank and suit, so we fix it as follow: before actually play that card, we should first double-check whether he has the card, if no then play the only card the player have(In this case, player should only have one card).

2. Pinochle Trial results:

Trial 1
$ ./lab3 Pinochle p1 p2 p3 p4
p1's hand:
QC QD QH 
JH 10C JC 
JC 9D 10C 
KD QS AH 
Melds:
Sixty Queens 60

p2's hand:
9H QD KH 
QS QH 9H 
10D 10D AC 
JD 9D KS 
Melds:
Pinochle 40

p3's hand:
9S AC 9C 
KC 10H KD 
JH JD AS 
9C AH JS 
Melds:

p4*'s hand:
10S KC AS 
AD 10H KH 
9S JS 10S 
AD KS QC 
Melds:

Trump suit: C
p1's bid: 133
p2's bid: 110
p3's bid: 67
p4*'s bid: 120
Team 1 bids: 200
Team 2 bids: 230
Team 2 won the bid.

Round 1
---------------
player p4* played AS
Trick: AS 
player p1 played QS
Trick: AS QS 
player p2 played QS
Trick: AS QS QS 
player p3 played 9S
Trick: AS QS QS 9S 
player p4* won the trick

Contract team won 17 points
Running tally: 117

p1's hand:
QC QD QH 
JH 10C JC 
JC 9D 10C 
KD AH 
Melds:

p2's hand:
9H QD KH 
QH 9H 10D 
10D AC JD 
9D KS 
Melds:

p3's hand:
AC 9C KC 
10H KD JH 
JD AS 9C 
AH JS 
Melds:

p4*'s hand:
10S KC AD 
10H KH 9S 
JS 10S AD 
KS QC 
Melds:

Round 2
---------------
player p4* played AD
Trick: AD 
player p1 played 9D
Trick: AD 9D 
player p2 played 9D
Trick: AD 9D 9D 
player p3 played JD
Trick: AD 9D 9D JD 
player p4* won the trick

Contract team won 13 points
Running tally: 130

p1's hand:
QC QD QH 
JH 10C JC 
JC 10C KD 
AH 
Melds:

p2's hand:
9H QD KH 
QH 9H 10D 
10D AC JD 
KS 
Melds:

p3's hand:
AC 9C KC 
10H KD JH 
AS 9C AH 
JS 
Melds:

p4*'s hand:
10S KC 10H 
KH 9S JS 
10S AD KS 
QC 
Melds:

Round 3
---------------
player p4* played AD
Trick: AD 
player p1 played QD
Trick: AD QD 
player p2 played JD
Trick: AD QD JD 
player p3 played KD
Trick: AD QD JD KD 
player p4* won the trick

Contract team won 20 points
Running tally: 150

p1's hand:
QC QH JH 
10C JC JC 
10C KD AH 
Melds:

p2's hand:
9H QD KH 
QH 9H 10D 
10D AC KS 
Melds:

p3's hand:
AC 9C KC 
10H JH AS 
9C AH JS 
Melds:

p4*'s hand:
10S KC 10H 
KH 9S JS 
10S KS QC 
Melds:

Round 4
---------------
player p4* played 10S
Trick: 10S 
player p1 played 10C
Trick: 10S 10C 
player p2 played KS
Trick: 10S 10C KS 
player p3 played JS
Trick: 10S 10C KS JS 
player p1 won the trick

Contract team failed, no points earned
Running tally: 150

p1's hand:
QC QH JH 
JC JC 10C 
KD AH 
Melds:

p2's hand:
9H QD KH 
QH 9H 10D 
10D AC 
Melds:

p3's hand:
AC 9C KC 
10H JH AS 
9C AH 
Melds:

p4*'s hand:
KC 10H KH 
9S JS 10S 
KS QC 
Melds:

Round 5
---------------
player p1 played AH
Trick: AH 
player p2 played 9H
Trick: AH 9H 
player p3 played JH
Trick: AH 9H JH 
player p4* played KH
Trick: AH 9H JH KH 
player p1 won the trick

Contract team failed, no points earned
Running tally: 150

p1's hand:
QC QH JH 
JC JC 10C 
KD 
Melds:

p2's hand:
QD KH QH 
9H 10D 10D 
AC 
Melds:

p3's hand:
AC 9C KC 
10H AS 9C 
AH 
Melds:

p4*'s hand:
KC 10H 9S 
JS 10S KS 
QC 
Melds:

Round 6
---------------
player p1 played 10C
Trick: 10C 
player p2 played AC
Trick: 10C AC 
player p3 played 9C
Trick: 10C AC 9C 
player p4* played QC
Trick: 10C AC 9C QC 
player p2 won the trick

Contract team won 24 points
Running tally: 174

p1's hand:
QC QH JH 
JC JC KD 
Melds:

p2's hand:
QD KH QH 
9H 10D 10D 
Melds:

p3's hand:
AC KC 10H 
AS 9C AH 
Melds:

p4*'s hand:
KC 10H 9S 
JS 10S KS 
Melds:

Round 7
---------------
player p2 played 10D
Trick: 10D 
player p3 played AC
Trick: 10D AC 
player p4* played 9S
Trick: 10D AC 9S 
player p1 played KD
Trick: 10D AC 9S KD 
player p3 won the trick

Contract team failed, no points earned
Running tally: 174

p1's hand:
QC QH JH 
JC JC 
Melds:

p2's hand:
QD KH QH 
9H 10D 
Melds:

p3's hand:
KC 10H AS 
9C AH 
Melds:

p4*'s hand:
KC 10H JS 
10S KS 
Melds:

Round 8
---------------
player p3 played AS
Trick: AS 
player p4* played JS
Trick: AS JS 
player p1 played QC
Trick: AS JS QC 
player p2 played 9H
Trick: AS JS QC 9H 
player p1 won the trick

Contract team failed, no points earned
Running tally: 174

p1's hand:
QH JH JC 
JC 
Melds:

p2's hand:
QD KH QH 
10D 
Melds:

p3's hand:
KC 10H 9C 
AH 
Melds:

p4*'s hand:
KC 10H 10S 
KS 
Melds:

Round 9
---------------
player p1 played QH
Trick: QH 
player p2 played KH
Trick: QH KH 
player p3 played AH
Trick: QH KH AH 
player p4* played 10H
Trick: QH KH AH 10H 
player p3 won the trick

Contract team failed, no points earned
Running tally: 174

p1's hand:
JH JC JC 
Melds:

p2's hand:
QD QH 10D 
Melds:

p3's hand:
KC 10H 9C 
Melds:

p4*'s hand:
KC 10S KS 
Melds:

Round 10
---------------
player p3 played 10H
Trick: 10H 
player p4* played KC
Trick: 10H KC 
player p1 played JH
Trick: 10H KC JH 
player p2 played QH
Trick: 10H KC JH QH 
player p4* won the trick

Contract team won 19 points
Running tally: 193

p1's hand:
JC JC 
Melds:

p2's hand:
QD 10D 
Melds:

p3's hand:
KC 9C 
Melds:

p4*'s hand:
10S KS 
Melds:

Round 11
---------------
player p4* played 10S
Trick: 10S 
player p1 played JC
Trick: 10S JC 
player p2 played QD
Trick: 10S JC QD 
player p3 played KC
Trick: 10S JC QD KC 
player p3 won the trick

Contract team failed, no points earned
Running tally: 193

p1's hand:
JC 
Melds:

p2's hand:
10D 
Melds:

p3's hand:
9C 
Melds:

p4*'s hand:
KS 
Melds:

Round 12
---------------
player p3 played 9C
Trick: 9C 
player p4* played KS
Trick: 9C KS 
player p1 played JC
Trick: 9C KS JC 
player p2 played 10D
Trick: 9C KS JC 10D 
player p1 won the trick

Contract team failed, no points earned
Running tally: 193

p1's hand:
Melds:

p2's hand:
Melds:

p3's hand:
Melds:

p4*'s hand:
Melds:

Contract failed!
Team 1 score: 0
Team 2 score: 0
End the game? (yes/no): 

3. HoldEm Bug fix:
    1) Big blind player has the risk of overflow due to the unsigned int type: eg. big blind got 1 chips left
        - Fix by checking all in status before each chips subtract

    2) Game process evaluation is tricky, when to continue bet, when to finish current round, when to end the game directly
        - Fix by set 3 evaluation status: continueBet, endRound, endGame
    
    3) The original operator< is not powerful enough to compare the equality of hand rank
        - Wrap the content with a operator helper function to return 4 status first: compareFail, equalRank, lowerThan, higherThan
        - Then turn it to only bool in the operator<
        - Thus it will ont influence the original operator< usage, we also got a way to know the equality between hands without redundant code

4. HoldEm Trial results:
    The entire output is too long, we just put some important state here:


    1) Entire game with all 4 rounds: preflop, flop, turn, river
End the game? (yes/no): no
----------------------New game!----------------------
player1's hand:
6H 3D 
player2*'s hand:
4S 4C 
player3's hand:
JC 6C 

----------Current round stats!----------
player1: score: 60, bet: 2, fold: 1, call: 1, raise times: 0
player2: score: 55, bet: 2, fold: 0, call: 1, raise times: 0
player3: score: 59, bet: 2, fold: 0, call: 1, raise times: 0
current pot: 6

BOARD(flop): 
AH JD 7H 

player3: JC 6C AH JD 7H (Pair)
player2*: 4S 4C AH JD 7H (Pair)
player1: 6H 3D AH JD 7H (X-High)

----------Current round stats!----------
player1: score: 60, bet: 0, fold: 1, call: 1, raise times: 0
player2: score: 55, bet: 0, fold: 0, call: 1, raise times: 0
player3: score: 59, bet: 0, fold: 0, call: 1, raise times: 0
current pot: 6

BOARD(turn): 
AH JD 7H 
9H 

player3: JC AH JD 7H 9H (Pair)
player2*: 4S 4C AH JD 9H (Pair)
player1: 6H AH JD 7H 9H (X-High)

----------Current round stats!----------
player1: score: 60, bet: 0, fold: 1, call: 1, raise times: 0
player2: score: 55, bet: 0, fold: 0, call: 1, raise times: 0
player3: score: 59, bet: 0, fold: 0, call: 1, raise times: 0
current pot: 6

BOARD(river): 
AH JD 7H 
9H 10H 

player1: 6H AH 7H 9H 10H (Flush)
player3: JC AH JD 9H 10H (Pair)
player2*: 4S 4C AH JD 10H (Pair)

----------Current round stats!----------
player1: score: 60, bet: 0, fold: 1, call: 1, raise times: 0
player2: score: 55, bet: 0, fold: 0, call: 1, raise times: 0
player3: score: 59, bet: 0, fold: 0, call: 1, raise times: 0
current pot: 6

----------Calculating scores!----------
player1 score: 60
player2 score: 55
player3 score: 65

End the game? (yes/no): 


    2) Short game when only left one unfold player in the preflop round
End the game? (yes/no): no
----------------------New game!----------------------
player1's hand:
9C QS 
player2's hand:
AS 2S 
player3*'s hand:
5D 3H 

----------Current round stats!----------
player1: score: 59, bet: 1, fold: 1, call: 1, raise times: 0
player2: score: 53, bet: 2, fold: 0, call: 0, raise times: 0
player3: score: 65, bet: 0, fold: 1, call: 1, raise times: 0
current pot: 3

----------Calculating scores!----------
player1 score: 59
player2 score: 56
player3 score: 65

End the game? (yes/no): 


    3) Short game when only left one unfold player after the flop round
End the game? (yes/no): no
----------------------New game!----------------------
player1's hand:
4H 2S 
player2's hand:
KD AS 
player3*'s hand:
6D 5C 

----------Current round stats!----------
player1: score: 54, bet: 1, fold: 1, call: 1, raise times: 0
player2: score: 62, bet: 2, fold: 0, call: 1, raise times: 0
player3: score: 59, bet: 2, fold: 0, call: 1, raise times: 0
current pot: 5

BOARD(flop): 
4S 7D 8D 

player3*: 6D 5C 4S 7D 8D (Straight)
player1: 4H 2S 4S 7D 8D (Pair)
player2: KD AS 4S 7D 8D (X-High)

----------Current round stats!----------
player1: score: 54, bet: 0, fold: 1, call: 1, raise times: 0
player2: score: 62, bet: 0, fold: 1, call: 1, raise times: 0
player3: score: 57, bet: 2, fold: 0, call: 0, raise times: 1
current pot: 7

----------Calculating scores!----------
player1 score: 54
player2 score: 62
player3 score: 64

End the game? (yes/no): 


    4) Game that someone continue raises until all in and one player got busted
End the game? (yes/no): no
----------------------New game!----------------------
player1's hand:
4D QD 
player2*'s hand:
2D JD 
player3's hand:
7H JH 

----------Current round stats!----------
player1: score: 64, bet: 2, fold: 0, call: 1, raise times: 0
player2: score: 50, bet: 2, fold: 0, call: 1, raise times: 0
player3: score: 60, bet: 2, fold: 0, call: 1, raise times: 0
current pot: 6

BOARD(flop): 
KD 7C 7S 

player3: 7H JH KD 7C 7S (Three of a Kind)
player1: 4D QD KD 7C 7S (Pair)
player2*: 2D JD KD 7C 7S (Pair)

----------Current round stats!----------
player1: score: 4, bet: 60, fold: 0, call: 1, raise times: 0
player2: score: 0, bet: 50, fold: 0, call: 1, raise times: 0
player3: score: 0, bet: 60, fold: 0, call: 1, raise times: 30
current pot: 176

BOARD(turn): 
KD 7C 7S 
3S 

player3: 7H JH KD 7C 7S (Three of a Kind)
player1: 4D QD KD 7C 7S (Pair)
player2*: JD KD 7C 7S 3S (Pair)

----------Current round stats!----------
player1: score: 4, bet: 0, fold: 0, call: 1, raise times: 0
player2: score: 0, bet: 0, fold: 0, call: 1, raise times: 0
player3: score: 0, bet: 0, fold: 0, call: 1, raise times: 0
current pot: 176

BOARD(river): 
KD 7C 7S 
3S QS 

player3: 7H JH KD 7C 7S (Three of a Kind)
player1: QD KD 7C 7S QS (Two Pair)
player2*: JD KD 7C 7S QS (Pair)

----------Current round stats!----------
player1: score: 4, bet: 0, fold: 0, call: 1, raise times: 0
player2: score: 0, bet: 0, fold: 0, call: 1, raise times: 0
player3: score: 0, bet: 0, fold: 0, call: 1, raise times: 0
current pot: 176

----------Calculating scores!----------
player1 score: 4
player2 score: 0
player3 score: 176

----------Busted out players!----------
player2: 3rd place

End the game? (yes/no): 


    5) Another player continue raises until all in to win the game
----------------------New game!----------------------
player1*'s hand:
2S 7S 
player3's hand:
6C 2C 

----------Current round stats!----------
player1: score: 2, bet: 2, fold: 0, call: 1, raise times: 0
player3: score: 174, bet: 2, fold: 0, call: 1, raise times: 0
current pot: 4

BOARD(flop): 
AH 8C AC 

player1*: 2S 7S AH 8C AC (Pair)
player3: 6C 2C AH 8C AC (Pair)

----------Current round stats!----------
player1: score: 2, bet: 0, fold: 0, call: 1, raise times: 0
player3: score: 174, bet: 0, fold: 0, call: 1, raise times: 0
current pot: 4

BOARD(turn): 
AH 8C AC 
3C 

player3: 6C 2C 8C AC 3C (Flush)
player1*: 7S AH 8C AC 3C (Pair)

----------Current round stats!----------
player1: score: 0, bet: 2, fold: 0, call: 1, raise times: 0
player3: score: 0, bet: 174, fold: 0, call: 0, raise times: 44
current pot: 180

----------Calculating scores!----------
player1 score: 0
player3 score: 180

----------Busted out players!----------
player1: 2nd place

----------Winner is player3!----------
Final chips: 180


    6) Rare case when there are multiple best hand, players split pot
End the game? (yes/no): no
----------------------New game!----------------------
player2*'s hand:
QC JC 
player3's hand:
QS JH 

----------Current round stats!----------
player2: score: 59, bet: 2, fold: 0, call: 1, raise times: 0
player3: score: 117, bet: 2, fold: 0, call: 1, raise times: 0
current pot: 4

BOARD(flop): 
9C 9S 6H 

player3: QS JH 9C 9S 6H (Pair)
player2*: QC JC 9C 9S 6H (Pair)

----------Current round stats!----------
player2: score: 59, bet: 0, fold: 0, call: 1, raise times: 0
player3: score: 117, bet: 0, fold: 0, call: 1, raise times: 0
current pot: 4

BOARD(turn): 
9C 9S 6H 
KD 

player3: QS JH 9C 9S KD (Pair)
player2*: QC JC 9C 9S KD (Pair)

----------Current round stats!----------
player2: score: 59, bet: 0, fold: 0, call: 1, raise times: 0
player3: score: 117, bet: 0, fold: 0, call: 1, raise times: 0
current pot: 4

BOARD(river): 
9C 9S 6H 
KD 8S 

player3: QS JH 9C 9S KD (Pair)
player2*: QC JC 9C 9S KD (Pair)

----------Current round stats!----------
player2: score: 59, bet: 0, fold: 0, call: 1, raise times: 0
player3: score: 117, bet: 0, fold: 0, call: 1, raise times: 0
current pot: 4

----------Calculating scores!----------
player2 score: 61
player3 score: 119

End the game? (yes/no):   