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

    const size_t numPreflopRound = 2;
    const size_t numCommonBoardsBatch = 3;
    const size_t numPlayerHands = 2;
    const size_t numFinalHands = 5;

    virtual void deal();
    void printHands();
    void printBoards();
    void printSortedHands();
    void collectHands();
    void collectBoards();

   public:
    struct HoldEmGameStruct {
        /* data */
        cardSetType cardSet;
        std::string playerName;
        HoldEmHandRank rank;
        size_t playerIdx;

        HoldEmGameStruct(cardSetType givenCardSet, std::string &givenPlayerName,
                         HoldEmHandRank givenRank, size_t playerIdx)
            : cardSet(givenCardSet),
              playerName(givenPlayerName),
              rank(givenRank), playerIdx(playerIdx) {}
    };

    HoldEmGame(int argc, const char **argv);

    bool static sortPair(const cardSetType &hand1, const cardSetType &hand2);
    bool static sortTwoPair(const cardSetType &hand1, const cardSetType &hand2);
    bool static sortAnyThree(const cardSetType &hand1, const cardSetType &hand2);
    bool static sortAnyStraight(const cardSetType &hand1, const cardSetType &hand2);
    bool static sortFlushXhigh(const cardSetType &hand1, const cardSetType &hand2);
    bool static sortFourOfAKind(const cardSetType &hand1, const cardSetType &hand2);

    virtual ~HoldEmGame() = default;
    virtual int play();

   private:
    HoldEmHandRank holdem_hand_evaluation(const cardSetType &hand);
    bool isFlush(const std::vector<cardType> &cards);
    bool isStraight(const std::vector<cardType> &cards);
    bool isFourOfAKind(const std::vector<cardType> &cards);
    bool isFullHouse(const std::vector<cardType> &cards);
    bool isThreeOfAKind(const std::vector<cardType> &cards);
    bool isTwoPair(const std::vector<cardType> &cards);
    bool isPair(const std::vector<cardType> &cards);
};

std::ostream &operator<<(std::ostream &os, const HoldEmHandRank &rank);
bool operator<(const HoldEmGame::HoldEmGameStruct &obj1,
               const HoldEmGame::HoldEmGameStruct &obj2);