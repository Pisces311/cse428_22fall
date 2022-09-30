#include "HoldEmDeck.h"

#include <iostream>
#include <vector>

#include "Card_T.h"
#include "Deck_T.h"
#include "Suit.h"

HoldEmDeck::HoldEmDeck() {
    for (Suit i = Suit::clubs; i <= Suit::spades; ++i) {
        for (HoldEmRank j = HoldEmRank::two; j <= HoldEmRank::ace; ++j) {
            HoldEmDeck::cards.push_back(Card<HoldEmRank, Suit>(j, i));
        }
    }
}

std::ostream& operator<<(std::ostream& os, const HoldEmRank& rank) {
    switch (rank) {
        case HoldEmRank::two:
            os << "2";
            break;
        case HoldEmRank::three:
            os << "3";
            break;
        case HoldEmRank::four:
            os << "4";
            break;
        case HoldEmRank::five:
            os << "5";
            break;
        case HoldEmRank::six:
            os << "6";
            break;
        case HoldEmRank::seven:
            os << "7";
            break;
        case HoldEmRank::eight:
            os << "8";
            break;
        case HoldEmRank::nine:
            os << "9";
            break;
        case HoldEmRank::ten:
            os << "10";
            break;
        case HoldEmRank::jack:
            os << "J";
            break;
        case HoldEmRank::queen:
            os << "Q";
            break;
        case HoldEmRank::king:
            os << "K";
            break;
        case HoldEmRank::ace:
            os << "A";
            break;
        case HoldEmRank::undefined:
            os << "?";
            break;
    }
    return os;
}

HoldEmRank& operator++(HoldEmRank& rank) {
    switch (rank) {
        case HoldEmRank::undefined:
            break;
        default:
            rank = HoldEmRank(static_cast<int>(rank) + 1);
            break;
    }
    return rank;
}