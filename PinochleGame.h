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
   protected:
    PinochleDeck deck;
    std::vector<CardSet<PinochleRank, Suit>> hands;

    virtual void deal();
    void printHands();
    void collectHands();

   public:
    static unsigned int points[];

    PinochleGame(int argc, const char** argv);
    virtual ~PinochleGame() = default;
    virtual int play();

   private:
    void suit_independent_evaluation(const CardSet<PinochleRank, Suit>& hand,
                                     std::vector<PinochleMelds>& melds);
};

std::ostream& operator<<(std::ostream& os, const PinochleMelds& meld);