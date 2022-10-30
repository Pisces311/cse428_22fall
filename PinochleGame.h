#pragma once
#include <vector>

#include "Game.h"
#include "PinochleDeck.h"

enum class PinochleMelds {
    dix,
    offsuitmarriage,
    fortyjacks,
    pinochle,
    insuitmarriage,
    sixtyqueens,
    eightykings,
    hundredaces,
    insuitrun,
    doublepinochle,
    fourhundredjacks,
    sixhundredqueens,
    eighthundredkings,
    thousandaces,
    insuitdoublerun
};

class PinochleGame : public Game {
   public:
    using cardType = Card<PinochleRank, Suit>;
    using cardSetType = CardSet<PinochleRank, Suit>;

    static unsigned int points[];

   protected:
    PinochleDeck deck;
    std::vector<cardSetType> hands;

    virtual void deal();
    void printHands();
    void collectHands();

   public:
    PinochleGame(int argc, const char** argv);
    virtual ~PinochleGame() = default;
    virtual int play();

   private:
    void suit_independent_evaluation(const cardSetType& hand,
                                     std::vector<PinochleMelds>& melds);
};

std::ostream& operator<<(std::ostream& os, const PinochleMelds& meld);