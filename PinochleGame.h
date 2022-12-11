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

    const int numInEachPacket = 3;
    const size_t sizeInEachPacket = 3;

    virtual void deal();
    void printHands();
    void collectHands();

   public:
    PinochleGame(int argc, const char** argv);
    virtual ~PinochleGame() = default;
    virtual int play();

   private:
    Suit trump_suit;
    unsigned int runningTally;
    int contractTeam;
    size_t firstPlayer;
    std::vector<unsigned int> teamBids;

    bool bidding(std::vector<unsigned int>& bids);
    void suit_independent_evaluation(const cardSetType& hand,
                                     std::vector<PinochleMelds>& melds);
    void suit_dependent_evaluation(const cardSetType& hand,
                                   std::vector<PinochleMelds>& melds,
                                   Suit suit);

    int hasSets(const std::vector<cardType>& cards, PinochleRank rank);

    bool isInsuitdoublerun(const std::vector<cardType>& cards, Suit suit);
    bool isInsuitrun(const std::vector<cardType>& cards, Suit suit);
    bool isInsuitmarriage(const std::vector<cardType>& cards, Suit suit);
    bool isOffsuitmarriage(const std::vector<cardType>& cards, Suit suit);
    bool isDix(const std::vector<cardType>& cards, Suit suit);

    int computeMelds(const cardSetType& hand);
    int computeCards(const cardSetType& hand);
    void playTricks();
    int playTrick(cardSetType& trick);

    Suit playFirstCard(std::vector<cardType>& trickCards, std::vector<cardType>& cards);
    bool hasSuit(const std::vector<cardType>& cards, Suit suit);
    bool playHighestCard(std::vector<cardType>& trickCards, std::vector<cardType>& cards, Suit suit);
    void playLowestCard(std::vector<cardType>& trickCards, std::vector<cardType>& cards, Suit suit);
    void playAnyLowestCard(std::vector<cardType>& trickCards, std::vector<cardType>& cards);

    bool compareCards(const cardType& card1, const cardType& card2, Suit leadingSuit);
    void addScore(int winnerTeam, const cardSetType& trick);

    int hasWinner();
};

std::ostream& operator<<(std::ostream& os, const PinochleMelds& meld);