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

enum class HoldEmRaiseCallState {
    raiseAlways,
    raiseOnce,
    callAlways,
    foldOrCheck,
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

    std::vector<bool> foldState;
    std::vector<bool> callState;
    std::vector<int> raiseTimes;
    std::vector<unsigned int> betChips;

    int initialScore = 60;
    const int smallBlindBet = 1;
    const int bigBlindBet = 2;
    int commonPot;

    const unsigned int smallRaise = 2;
    const unsigned int bigRaise = 4;

    const int continueBet = -1;
    const int endRound = 0;
    const int endGame = 1;

    const size_t numPreflopRound = 2;
    const size_t numCommonBoardsBatch = 3;
    const size_t numPlayerHands = 2;
    const size_t numFinalHands = 5;
    const size_t numCardsAfterTurn = 6;
    const size_t numCardsAfterRiver = 7;

    virtual void deal();
    void printHands();
    void printBoards();
    void printSortedHands();
    void printRoundStats();
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

    const static int compareFail = -2;
    const static int equalRank = -1;
    const static int lowerThan = 1;
    const static int higherThan = 0;

    int static sortPair(const cardSetType &hand1, const cardSetType &hand2);
    int static sortTwoPair(const cardSetType &hand1, const cardSetType &hand2);
    int static sortAnyThree(const cardSetType &hand1, const cardSetType &hand2);
    int static sortAnyStraight(const cardSetType &hand1, const cardSetType &hand2);
    int static sortFlushXhigh(const cardSetType &hand1, const cardSetType &hand2);
    int static sortFourOfAKind(const cardSetType &hand1, const cardSetType &hand2);

    virtual ~HoldEmGame() = default;
    virtual int play();

   private:
    std::vector<HoldEmGameStruct> structs;

    bool endingGame();
    void calculateScore();
    int evaluateEndRoundOrGame();
    void processChips(const int &currPlayerIdx, const unsigned int &diffBet);
    void addToPot();
    void actRaiseOrCall(const HoldEmRaiseCallState &raiseCallState, const int &currPlayerIdx);
    bool bet();
    bool preflopBet();
    bool postflopBet();
    HoldEmRaiseCallState evaluatePreflopState(const cardSetType &hand);
    HoldEmRaiseCallState evaluatePostFlopState(const HoldEmGameStruct &currStruct);
    HoldEmRaiseCallState evaluatePostTurnState(const HoldEmGameStruct &currStruct);
    HoldEmRaiseCallState evaluatePostRiverState(const HoldEmGameStruct &currStruct);
    HoldEmRaiseCallState evaluateFourCards(const HoldEmGameStruct &currStruct);

    HoldEmGameStruct findBestHand(const cardSetType &hand, std::string &givenPlayerName, size_t playerIdx);
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
int operaterHelper(const HoldEmGame::HoldEmGameStruct &obj1,
               const HoldEmGame::HoldEmGameStruct &obj2);