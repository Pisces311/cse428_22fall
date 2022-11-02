#pragma once
#include <string>
#include <vector>

#include "Game.h"
#include "HoldEmDeck.h"

enum class HoldEmState { preflop, flop, turn, river, undefined };

enum class HoldEmHandRank {
    xhigh,
    pair,
    twopair,
    threeofakind,
    straight,
    flush,
    fullhouse,
    fourofakind,
    straightflush,
    undefined
};

class HoldEmGame : public Game {
   public:
    using cardType = Card<HoldEmRank, Suit>;
    using cardSetType = CardSet<HoldEmRank, Suit>;

   protected:
    HoldEmState state;
    HoldEmDeck deck;
    std::vector<cardSetType> hands;
    cardSetType commonBoards;

    virtual void deal();
    void printHands();
    void printBoards();
    void collectHands();
    void collectBoards();

   public:
    struct HoldEmGameStruct {
        /* data */
        cardSetType cardSet;
        std::string playerName;
        HoldEmHandRank rank;

        HoldEmGameStruct(cardSetType givenCardSet, std::string &givenPlayerName,
                         HoldEmHandRank givenRank)
            : cardSet(givenCardSet),
              playerName(givenPlayerName),
              rank(givenRank) {}
    };

    HoldEmGame(int argc, const char **argv);
    virtual ~HoldEmGame() = default;
    virtual int play();

   private:
    HoldEmHandRank holdem_hand_evaluation(const cardSetType &hand);
};

std::ostream &operator<<(std::ostream &os, const HoldEmHandRank &rank);
bool operator<(const HoldEmGame::HoldEmGameStruct &obj1,
               const HoldEmGame::HoldEmGameStruct &obj2);