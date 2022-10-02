#include <vector>

#include "Game.h"
#include "PinochleDeck.h"

class PinochleGame : public Game {
   protected:
    PinochleDeck deck;
    std::vector<CardSet<PinochleRank, Suit>> hands;

    virtual void deal();
    void printHands();
    void collectHands();

   public:
    PinochleGame(int argc, const char **argv);
    virtual ~PinochleGame() = default;
    virtual int play();
};