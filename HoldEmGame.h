#include <vector>

#include "Game.h"
#include "HoldEmDeck.h"

class HoldEmGame : public Game {
   protected:
    HoldEmDeck deck;
    std::vector<CardSet<HoldEmRank, Suit>> hands;

    // void deal();
    void printHands();
    void collectHands();

   public:
    HoldEmGame(int argc, const char **argv);
    virtual ~HoldEmGame() = default;
    // virtual int play();
};