#include <vector>

#include "Game.h"
#include "HoldEmDeck.h"

enum class HoldEmState { preflop, flop, turn, river, undefined };

class HoldEmGame : public Game {
   protected:
    HoldEmState state;
    HoldEmDeck deck;
    std::vector<CardSet<HoldEmRank, Suit>> hands;
    CardSet<HoldEmRank, Suit> commonBoards;

    virtual void deal();
    void printHands();
    void printBoards();
    void collectHands();
    void collectBoards();

   public:
    HoldEmGame(int argc, const char **argv);
    virtual ~HoldEmGame() = default;
    virtual int play();
};