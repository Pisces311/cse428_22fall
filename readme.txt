lab3
Ke Peng k.peng@wustl.edu
Zichao Liang liang.zichao@wustl.edu
Siqian Hou h.siqian@wustl.edu

1. Bug fix:
    1) One of the bugs in Pinochle we meet is when a player's highest card has the same rank and suit as the highest card in trick, the player will still play it instead of play a lowest card. The reason is that when we compare the highest hand card with the trick, we originally use > instead of of >=., so when the cards are the same, bug will happen.

    2) Another bug from Pinochle is when the game approach the end, the player may play a card that he actually don't have. Since when we are finding the highest card a player should play, we will set a default value as the lowest card rank and suit, so we fix it as follow: before actually play that card, we should first double-check whether he has the card, if no then play the only card the player have(In this case, player should only have one card).

2. Trial results:

Trial 1
$ ./lab3 Pinochle p1 p2 p3 p4