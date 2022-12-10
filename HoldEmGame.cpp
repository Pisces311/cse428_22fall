#include "HoldEmGame.h"

#include <algorithm>

HoldEmGame::HoldEmGame(int argc, const char **argv) : Game(argc, argv) {
    state = HoldEmState::preflop;
    for (int i = startIndex; i < argc; ++i) {
        hands.push_back(cardSetType());
    }
}

void HoldEmGame::deal() {
    int cardToSend = numPreflopRound * players.size();
    int playerIdx = (dealer + 1) % players.size();
    switch (state) {
        case HoldEmState::preflop:
            while (cardToSend--) {
                deck >> hands[playerIdx];
                playerIdx = (playerIdx + 1) % players.size();
            }
            state = HoldEmState(static_cast<int>(state) + 1);
            break;
        case HoldEmState::flop:
            for (size_t i = 0; i < numCommonBoardsBatch; i++) {
                deck >> commonBoards;
            }
            state = HoldEmState(static_cast<int>(state) + 1);
            break;
        case HoldEmState::turn:
        case HoldEmState::river:
            deck >> commonBoards;
            state = HoldEmState(static_cast<int>(state) + 1);
            break;
        default:
            break;
    }
}

void HoldEmGame::printHands() {
    for (size_t i = 0; i < hands.size(); i++) {
        std::cout << players[i];
        if (i == dealer) {
            std::cout << "*";
        }
        std::cout << "'s hand:" << std::endl;
        hands[i].print(std::cout, numPlayerHands);
    }
}

void HoldEmGame::printBoards() {
    commonBoards.print(std::cout, numCommonBoardsBatch);
}

void HoldEmGame::printSortedHands() {
    std::vector<HoldEmGameStruct> structs;
    for (size_t i = 0; i < players.size(); i++) {
        structs.emplace_back(hands[i], players[i], HoldEmHandRank::undefined,
                             i);
    }
    std::vector<cardType> cardSetType::*cards = cardSetType::getCards();
    for (size_t i = 0; i < structs.size(); i++) {
        for (size_t j = 0; j < (commonBoards.*cards).size(); j++) {
            ((structs[i].cardSet).*cards).push_back((commonBoards.*cards)[j]);
        }
        structs[i].rank = holdem_hand_evaluation(structs[i].cardSet);
    }
    // sort by player's cardset rank
    std::sort(structs.begin(), structs.end(), operator<);
    std::reverse(structs.begin(), structs.end());
    std::cout << "" << std::endl;
    for (size_t i = 0; i < structs.size(); i++) {
        std::cout << structs[i].playerName;
        if (structs[i].playerIdx == dealer) {
            std::cout << "*";
        }
        std::cout << ": ";
        for (size_t j = 0; j < ((structs[i].cardSet).*cards).size(); j++) {
            std::cout << ((structs[i].cardSet).*cards)[j] << " ";
        }
        std::cout << "(" << structs[i].rank << ")" << std::endl;
    }
    std::cout << "" << std::endl;
}

void HoldEmGame::collectHands() {
    for (cardSetType &hand : hands) {
        deck.collect(hand);
    }
}

void HoldEmGame::collectBoards() { deck.collect(commonBoards); }

int HoldEmGame::play() {
    while (true) {
        deck.shuffle();
        state = HoldEmState::preflop;
        deal();
        printHands();

        deal();
        std::cout << "BOARD(flop): " << std::endl;
        printBoards();

        printSortedHands();

        deal();
        std::cout << "BOARD(turn): " << std::endl;
        printBoards();

        deal();
        std::cout << "BOARD(river): " << std::endl;
        printBoards();

        collectHands();
        collectBoards();

        if (continuePrompt()) {
            break;
        }
        dealer = (dealer + 1) % players.size();
    }
    return SUCCESS;
}

HoldEmHandRank HoldEmGame::holdem_hand_evaluation(const cardSetType &hand) {
    cardSetType handCopy = hand;
    std::vector<cardType> cardSetType::*cards = cardSetType::getCards();

    // undefined
    if ((handCopy.*cards).size() != numFinalHands) {
        return HoldEmHandRank::undefined;
    }

    std::sort((handCopy.*cards).begin(), (handCopy.*cards).end(),
              compareByRank<HoldEmRank, Suit>);

    bool flush = isFlush(handCopy.*cards);
    bool straight = isStraight(handCopy.*cards);

    // straight flush
    if (flush && straight) {
        return HoldEmHandRank::straightflush;
    }

    // four of a kind
    if (isFourOfAKind(handCopy.*cards)) {
        return HoldEmHandRank::fourofakind;
    }

    // full house
    if (isFullHouse(handCopy.*cards)) {
        return HoldEmHandRank::fullhouse;
    }

    // flush
    if (flush) {
        return HoldEmHandRank::flush;
    }

    // straight
    if (straight) {
        return HoldEmHandRank::straight;
    }

    // three of a kind
    if (isThreeOfAKind(handCopy.*cards)) {
        return HoldEmHandRank::threeofakind;
    }

    // two pair
    if (isTwoPair(handCopy.*cards)) {
        return HoldEmHandRank::twopair;
    }

    // pair
    if (isPair(handCopy.*cards)) {
        return HoldEmHandRank::pair;
    }

    // xhigh
    return HoldEmHandRank::xhigh;
}

bool HoldEmGame::isFlush(const std::vector<cardType> &cards) {
    for (size_t i = 1; i < cards.size(); i++) {
        if (cards[i].suit != cards[i - 1].suit) {
            return false;
        }
    }
    return true;
}

bool HoldEmGame::isStraight(const std::vector<cardType> &cards) {
    for (size_t i = 1; i < cards.size(); i++) {
        int prevRank = static_cast<int>(cards[i - 1].rank);
        int currRank = static_cast<int>(cards[i].rank);
        if (prevRank + 1 != currRank &&
            !(cards[i - 1].rank == HoldEmRank::five &&
              cards[i].rank == HoldEmRank::ace)) {
            return false;
        }
    }
    return true;
}

bool HoldEmGame::isFourOfAKind(const std::vector<cardType> &cards) {
    return cards[0].rank == cards[3].rank || cards[1].rank == cards[4].rank;
}

bool HoldEmGame::isFullHouse(const std::vector<cardType> &cards) {
    return (cards[0].rank == cards[1].rank && cards[2].rank == cards[4].rank) ||
           (cards[0].rank == cards[2].rank && cards[3].rank == cards[4].rank);
}

bool HoldEmGame::isThreeOfAKind(const std::vector<cardType> &cards) {
    return cards[0].rank == cards[2].rank || cards[1].rank == cards[3].rank ||
           cards[2].rank == cards[4].rank;
}

bool HoldEmGame::isTwoPair(const std::vector<cardType> &cards) {
    return (cards[0].rank == cards[1].rank && cards[2].rank == cards[3].rank) ||
           (cards[0].rank == cards[1].rank && cards[3].rank == cards[4].rank) ||
           (cards[1].rank == cards[2].rank && cards[3].rank == cards[4].rank);
}

bool HoldEmGame::isPair(const std::vector<cardType> &cards) {
    return cards[0].rank == cards[1].rank || cards[1].rank == cards[2].rank ||
           cards[2].rank == cards[3].rank || cards[3].rank == cards[4].rank;
}

std::ostream &operator<<(std::ostream &os, const HoldEmHandRank &rank) {
    switch (rank) {
        case HoldEmHandRank::xhigh:
            os << "X-High";
            break;
        case HoldEmHandRank::pair:
            os << "Pair";
            break;
        case HoldEmHandRank::twopair:
            os << "Two Pair";
            break;
        case HoldEmHandRank::threeofakind:
            os << "Three of a Kind";
            break;
        case HoldEmHandRank::straight:
            os << "Straight";
            break;
        case HoldEmHandRank::flush:
            os << "Flush";
            break;
        case HoldEmHandRank::fullhouse:
            os << "Full House";
            break;
        case HoldEmHandRank::fourofakind:
            os << "Four of a Kind";
            break;
        case HoldEmHandRank::straightflush:
            os << "Straight Flush";
            break;
        default:
            os << "Undefined";
            break;
    }
    return os;
}

bool operator<(const HoldEmGame::HoldEmGameStruct &obj1,
               const HoldEmGame::HoldEmGameStruct &obj2) {
    using cardType = Card<HoldEmRank, Suit>;
    using cardSetType = CardSet<HoldEmRank, Suit>;

    if (obj1.rank < obj2.rank) {
        return true;
    } else if (obj1.rank > obj2.rank) {
        return false;
    }

    cardSetType hand1 = obj1.cardSet;
    cardSetType hand2 = obj2.cardSet;
    std::vector<cardType> cardSetType::*cards = cardSetType::getCards();

    // Sort by rank from A-2 high to low
    std::sort((hand1.*cards).begin(), (hand1.*cards).end(),
              compareByRank<HoldEmRank, Suit>);
    std::reverse((hand1.*cards).begin(), (hand1.*cards).end());
    std::sort((hand2.*cards).begin(), (hand2.*cards).end(),
              compareByRank<HoldEmRank, Suit>);
    std::reverse((hand2.*cards).begin(), (hand2.*cards).end());

    // Both pair
    if (obj1.rank == HoldEmHandRank::pair) {
        return HoldEmGame::sortPair(hand1, hand2);
    }

    // Both twopair
    if (obj1.rank == HoldEmHandRank::twopair) {
        return HoldEmGame::sortTwoPair(hand1, hand2);
    }

    // Both threeofakind or fullhouse
    if (obj1.rank == HoldEmHandRank::threeofakind ||
        obj1.rank == HoldEmHandRank::fullhouse) {
        return HoldEmGame::sortAnyThree(hand1, hand2);
    }

    // Both straight or straightflush
    if (obj1.rank == HoldEmHandRank::straight ||
        obj1.rank == HoldEmHandRank::straightflush) {
        return HoldEmGame::sortAnyStraight(hand1, hand2);
    }

    // Both flush or xhigh
    if (obj1.rank == HoldEmHandRank::flush ||
        obj1.rank == HoldEmHandRank::xhigh) {
        return HoldEmGame::sortFlushXhigh(hand1, hand2);
    }

    // Both fourofakind
    if (obj1.rank == HoldEmHandRank::fourofakind) {
        return HoldEmGame::sortFourOfAKind(hand1, hand2);
    }

    return false;
}

bool HoldEmGame::sortPair(const cardSetType &hand1, const cardSetType &hand2) {
    std::vector<cardType> cardSetType::*cards = cardSetType::getCards();
    cardSetType handCopy1 = hand1;
    cardSetType handCopy2 = hand2;
    // get the pair rank
    HoldEmRank pairRank1, pairRank2;
    for (size_t i = 1; i < (hand1.*cards).size(); i++) {
        if ((hand1.*cards)[i - 1].rank == (hand1.*cards)[i].rank) {
            pairRank1 = (hand1.*cards)[i].rank;
            // remove pair cards
            (handCopy1.*cards)
                .erase((handCopy1.*cards).begin() + (i - 1),
                       (handCopy1.*cards).begin() + (i + 1));
        }
        if ((hand2.*cards)[i - 1].rank == (hand2.*cards)[i].rank) {
            pairRank2 = (hand2.*cards)[i].rank;
            // remove pair cards
            (handCopy2.*cards)
                .erase((handCopy2.*cards).begin() + (i - 1),
                       (handCopy2.*cards).begin() + (i + 1));
        }
    }
    // compare pair rank
    if (pairRank1 < pairRank2) {
        return true;
    } else if (pairRank1 > pairRank2) {
        return false;
    }
    // compare other cards rank from high to low
    else {
        for (size_t i = 0; i < (handCopy1.*cards).size(); i++) {
            if ((handCopy1.*cards)[i].rank < (handCopy2.*cards)[i].rank) {
                return true;
            } else if ((handCopy1.*cards)[i].rank >
                       (handCopy2.*cards)[i].rank) {
                return false;
            }
        }
    }
    return false;
}

bool HoldEmGame::sortTwoPair(const cardSetType &hand1,
                             const cardSetType &hand2) {
    std::vector<cardType> cardSetType::*cards = cardSetType::getCards();
    // get the two pairs rank
    std::vector<HoldEmRank> pairRank1, pairRank2;
    for (size_t i = 1; i < (hand1.*cards).size(); i++) {
        if ((hand1.*cards)[i - 1].rank == (hand1.*cards)[i].rank) {
            pairRank1.push_back((hand1.*cards)[i].rank);
        }
        if ((hand2.*cards)[i - 1].rank == (hand2.*cards)[i].rank) {
            pairRank2.push_back((hand2.*cards)[i].rank);
        }
    }
    // compare pair ranks from high to low
    for (size_t i = 0; i < pairRank1.size(); i++) {
        if (pairRank1[i] < pairRank2[i]) {
            return true;
        } else if (pairRank1[i] > pairRank2[i]) {
            return false;
        }
    }
    // get the other card
    HoldEmRank nonPairRank1, nonPairRank2;
    for (size_t i = 0; i < (hand1.*cards).size(); i++) {
        if ((hand1.*cards)[i].rank != pairRank1[0] &&
            (hand1.*cards)[i].rank != pairRank1[1]) {
            nonPairRank1 = (hand1.*cards)[i].rank;
        }
        if ((hand2.*cards)[i].rank != pairRank2[0] &&
            (hand2.*cards)[i].rank != pairRank2[1]) {
            nonPairRank2 = (hand2.*cards)[i].rank;
        }
    }
    // compare the other card rank
    if (nonPairRank1 < nonPairRank2) {
        return true;
    }
    return false;
}

bool HoldEmGame::sortAnyThree(const cardSetType &hand1,
                              const cardSetType &hand2) {
    std::vector<cardType> cardSetType::*cards = cardSetType::getCards();
    // get the three of a kind rank
    HoldEmRank pairRank1, pairRank2;
    for (size_t i = 2; i < (hand1.*cards).size(); i++) {
        if ((hand1.*cards)[i - 1].rank == (hand1.*cards)[i].rank &&
            (hand1.*cards)[i - 2].rank == (hand1.*cards)[i].rank) {
            pairRank1 = (hand1.*cards)[i].rank;
        }
        if ((hand2.*cards)[i - 1].rank == (hand2.*cards)[i].rank &&
            (hand2.*cards)[i - 2].rank == (hand2.*cards)[i].rank) {
            pairRank2 = (hand2.*cards)[i].rank;
        }
    }
    // compare pair rank
    if (pairRank1 < pairRank2) {
        return true;
    }
    return false;
}

bool HoldEmGame::sortAnyStraight(const cardSetType &hand1,
                                 const cardSetType &hand2) {
    std::vector<cardType> cardSetType::*cards = cardSetType::getCards();
    if ((hand1.*cards)[0].rank < (hand2.*cards)[0].rank) {
        return true;
    }
    return false;
}

bool HoldEmGame::sortFlushXhigh(const cardSetType &hand1,
                                const cardSetType &hand2) {
    std::vector<cardType> cardSetType::*cards = cardSetType::getCards();
    for (size_t i = 0; i < (hand1.*cards).size(); i++) {
        if ((hand1.*cards)[i].rank < (hand2.*cards)[i].rank) {
            return true;
        } else if ((hand1.*cards)[i].rank > (hand2.*cards)[i].rank) {
            return false;
        }
    }
    return false;
}

bool HoldEmGame::sortFourOfAKind(const cardSetType &hand1,
                                 const cardSetType &hand2) {
    std::vector<cardType> cardSetType::*cards = cardSetType::getCards();
    // get pair
    HoldEmRank pairRank1 = (hand1.*cards)[1].rank;
    HoldEmRank pairRank2 = (hand2.*cards)[1].rank;
    // compare pair rank
    if (pairRank1 < pairRank2) {
        return true;
    }
    return false;
}