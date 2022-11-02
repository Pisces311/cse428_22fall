#include "HoldEmGame.h"

#include <algorithm>

using cardType = Card<HoldEmRank, Suit>;
using cardSetType = CardSet<HoldEmRank, Suit>;

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

        std::vector<HoldEmGameStruct> structs;
        for (size_t i = 0; i < players.size(); i++) {
            structs.push_back(HoldEmGameStruct(hands[i], players[i], HoldEmHandRank::undefined));
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
        for (size_t i = 0; i < structs.size(); i++) {
            std::cout << structs[i].playerName << "'s rank: " << structs[i].rank << std::endl;
            if (structs[0] < structs[1]) {
                std::cout << "yes" << std::endl;
            }
        }



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

bool operator<(const HoldEmGame::HoldEmGameStruct &obj1, const HoldEmGame::HoldEmGameStruct &obj2) {
    if (obj1.rank < obj2.rank)
    {
        return true;
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
    if (obj1.rank == HoldEmHandRank::pair && obj1.rank == obj2.rank) {
        // get the pair rank
        HoldEmRank pairRank1, pairRank2;
        std::vector<cardType>::const_iterator first1, last1, first2, last2;
        for (size_t i = 1; i < (hand1.*cards).size(); i++) {
            if ((hand1.*cards)[i-1].rank == (hand1.*cards)[i].rank) {
                pairRank1 = (hand1.*cards)[i].rank;
                first1 = (hand1.*cards).begin() + (i-1);
                last1 = (hand1.*cards).begin() + i;
            }
            if ((hand2.*cards)[i-1].rank == (hand2.*cards)[i].rank) {
                pairRank2 = (hand2.*cards)[i].rank;
                first2 = (hand2.*cards).begin() + (i-1);
                last2 = (hand2.*cards).begin() + i;
            }
        }
        // compare pair rank
        if (pairRank1 < pairRank2) {
            return true;
        }
        // compare other cards rank from high to low
        else {
            // get the other cards
            std::vector<cardType> nonPairHand1(first1, last1);
            std::vector<cardType> nonPairHand2(first2, last2);
            // compare
            for (size_t i = 0; i < nonPairHand1.size(); i++) {
                if (nonPairHand1[i].rank < nonPairHand2[i].rank) {
                    return true;
                }
            }
        }
    }

    // Both twopair
    if (obj1.rank == HoldEmHandRank::twopair && obj1.rank == obj2.rank) {
        // get the two pairs rank
        std::vector<HoldEmRank> pairRank1, pairRank2;
        for (size_t i = 1; i < (hand1.*cards).size(); i++) {
            if ((hand1.*cards)[i-1].rank == (hand1.*cards)[i].rank) {
                pairRank1.push_back((hand1.*cards)[i].rank);
            }
            if ((hand2.*cards)[i-1].rank == (hand2.*cards)[i].rank) {
                pairRank2.push_back((hand2.*cards)[i].rank);
            }
        }
        // compare pair ranks from high to low
        for (size_t i = 0; i < pairRank1.size(); i++) {
            if (pairRank1[i] < pairRank2[i]) {
                return true;
            }
        }
        // get the otehr card
        HoldEmRank nonPairRank1, nonPairRank2;
        for (size_t i = 0; i < (hand1.*cards).size(); i++) {
            if (std::find(pairRank1.begin(), pairRank1.end(), (hand1.*cards)[i].rank) 
                == pairRank1.end()) {
                nonPairRank1 = (hand1.*cards)[i].rank;
            }
            if (std::find(pairRank2.begin(), pairRank2.end(), (hand2.*cards)[i].rank) 
                == pairRank2.end()) {
                nonPairRank2 = (hand2.*cards)[i].rank;
            }
        }
        // compare the other card rank
        if (nonPairRank1 < nonPairRank2) {
            return true;
        }
    }
    
    // Both threeofakind or fullhouse
    if ((obj1.rank == HoldEmHandRank::threeofakind || obj1.rank == HoldEmHandRank::fullhouse) 
        && (obj1.rank == obj2.rank)) {
        // get the three of a kind rank
        HoldEmRank pairRank1, pairRank2;
        for (size_t i = 2; i < (hand1.*cards).size(); i++) {
            if ((hand1.*cards)[i-1].rank == (hand1.*cards)[i].rank 
                && (hand1.*cards)[i-2].rank == (hand1.*cards)[i].rank) {
                pairRank1 = (hand1.*cards)[i].rank;
            }
            if ((hand2.*cards)[i-1].rank == (hand2.*cards)[i].rank 
                && (hand2.*cards)[i-2].rank == (hand2.*cards)[i].rank) {
                pairRank2 = (hand2.*cards)[i].rank;
            }
        }
        // compare pair rank
        if (pairRank1 < pairRank2) {
            return true;
        }
    }

    // Both straight or straightflush
    if ((obj1.rank == HoldEmHandRank::straight || obj1.rank == HoldEmHandRank::straightflush) 
        && (obj1.rank == obj2.rank)) {
        if ((hand1.*cards)[0].rank < (hand2.*cards)[0].rank) {
            return true;
        }
    }

    // Both flush or xhigh
    if ((obj1.rank == HoldEmHandRank::flush || obj1.rank == HoldEmHandRank::xhigh) 
        && (obj1.rank == obj2.rank)) {
        for (size_t i = 0; i < (hand1.*cards).size(); i++) {
            if ((hand1.*cards)[i].rank < (hand2.*cards)[i].rank) {
                return true;
            }
        }
    }

    // Both fourofakind
    if (obj1.rank == HoldEmHandRank::fourofakind && obj1.rank == obj2.rank) {
        // get paire
        HoldEmRank pairRank1 = (hand1.*cards)[1].rank;
        HoldEmRank pairRank2 = (hand2.*cards)[1].rank;
        // compare pair rank
        if (pairRank1 < pairRank2) {
            return true;
        }
    }

    return false;
}