#include "HoldEmGame.h"

#include <algorithm>

HoldEmGame::HoldEmGame(int argc, const char** argv) : Game(argc, argv) {
    state = HoldEmState::preflop;
    for (int i = startIndex; i < argc; ++i) {
        hands.push_back(cardSetType());
    }
}

void HoldEmGame::deal() {
    switch (state) {
        case HoldEmState::preflop:
            for (size_t i = 0; i < 2; i++) {
                for (size_t j = 0; j < hands.size(); j++) {
                    deck >> hands[j];
                }
            }
            state = HoldEmState(static_cast<int>(state) + 1);
            break;
        case HoldEmState::flop:
            for (size_t i = 0; i < 3; i++) {
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
        std::cout << players[i] << "'s hand:" << std::endl;
        hands[i].print(std::cout, 2);
    }
}

void HoldEmGame::printBoards() { commonBoards.print(std::cout, 3); }

void HoldEmGame::collectHands() {
    for (cardSetType& hand : hands) {
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
    }
    return SUCCESS;
}

HoldEmHandRank HoldEmGame::holdem_hand_evaluation(const cardSetType& hand) {
    cardSetType handCopy = hand;
    std::vector<cardType> cardSetType::*cards = cardSetType::getCards();

    // undefined
    if ((handCopy.*cards).size() != 5) {
        return HoldEmHandRank::undefined;
    }

    bool isFlush = true;
    for (size_t i = 1; i < (handCopy.*cards).size(); i++) {
        if ((handCopy.*cards)[i].suit != (handCopy.*cards)[i - 1].suit) {
            isFlush = false;
            break;
        }
    }

    std::sort((handCopy.*cards).begin(), (handCopy.*cards).end(),
              compareByRank<HoldEmRank, Suit>);
    bool isStraight = true;
    for (size_t i = 1; i < (handCopy.*cards).size(); i++) {
        int prevRank = static_cast<int>((handCopy.*cards)[i - 1].rank);
        int currRank = static_cast<int>((handCopy.*cards)[i].rank);
        if (prevRank + 1 != currRank &&
            !((handCopy.*cards)[i - 1].rank == HoldEmRank::five &&
              (handCopy.*cards)[i].rank == HoldEmRank::ace)) {
            isStraight = false;
            break;
        }
    }

    // straight flush
    if (isFlush && isStraight) {
        return HoldEmHandRank::straightflush;
    }

    // four of a kind
    if ((handCopy.*cards)[0].rank == (handCopy.*cards)[3].rank ||
        (handCopy.*cards)[1].rank == (handCopy.*cards)[4].rank) {
        return HoldEmHandRank::fourofakind;
    }

    // full house
    if (((handCopy.*cards)[0].rank == (handCopy.*cards)[1].rank &&
         (handCopy.*cards)[2].rank == (handCopy.*cards)[4].rank) ||
        ((handCopy.*cards)[0].rank == (handCopy.*cards)[2].rank &&
         (handCopy.*cards)[3].rank == (handCopy.*cards)[4].rank)) {
        return HoldEmHandRank::fullhouse;
    }

    // flush
    if (isFlush) {
        return HoldEmHandRank::flush;
    }

    // straight
    if (isStraight) {
        return HoldEmHandRank::straight;
    }

    // three of a kind
    if ((handCopy.*cards)[0].rank == (handCopy.*cards)[2].rank ||
        (handCopy.*cards)[1].rank == (handCopy.*cards)[3].rank ||
        (handCopy.*cards)[2].rank == (handCopy.*cards)[4].rank) {
        return HoldEmHandRank::threeofakind;
    }

    // two pair
    if (((handCopy.*cards)[0].rank == (handCopy.*cards)[1].rank &&
         (handCopy.*cards)[2].rank == (handCopy.*cards)[3].rank) ||
        ((handCopy.*cards)[0].rank == (handCopy.*cards)[1].rank &&
         (handCopy.*cards)[3].rank == (handCopy.*cards)[4].rank) ||
        ((handCopy.*cards)[1].rank == (handCopy.*cards)[2].rank &&
         (handCopy.*cards)[3].rank == (handCopy.*cards)[4].rank)) {
        return HoldEmHandRank::twopair;
    }

    // pair
    if ((handCopy.*cards)[0].rank == (handCopy.*cards)[1].rank ||
        (handCopy.*cards)[1].rank == (handCopy.*cards)[2].rank ||
        (handCopy.*cards)[2].rank == (handCopy.*cards)[3].rank ||
        (handCopy.*cards)[3].rank == (handCopy.*cards)[4].rank) {
        return HoldEmHandRank::pair;
    }

    // xhigh
    return HoldEmHandRank::xhigh;
}

std::ostream& operator<<(std::ostream& os, const HoldEmHandRank& rank) {
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