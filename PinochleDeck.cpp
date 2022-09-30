#include "PinochleDeck.h"

#include <iostream>
#include <vector>

#include "Card_T.h"
#include "Deck_T.h"
#include "Suit.h"

PinochleDeck::PinochleDeck() {
    for (size_t i = 0; i < numDecks; i++) {
        for (Suit j = Suit::clubs; j <= Suit::spades; ++j) {
            for (PinochleRank k = PinochleRank::nine; k <= PinochleRank::ace;
                 ++k) {
                PinochleDeck::cards.push_back(Card<PinochleRank, Suit>(k, j));
            }
        }
    }
}

std::ostream& operator<<(std::ostream& os, const PinochleRank& rank) {
    switch (rank) {
        case PinochleRank::nine:
            os << "9";
            break;
        case PinochleRank::jack:
            os << "J";
            break;
        case PinochleRank::queen:
            os << "Q";
            break;
        case PinochleRank::king:
            os << "K";
            break;
        case PinochleRank::ten:
            os << "10";
            break;
        case PinochleRank::ace:
            os << "A";
            break;
        case PinochleRank::undefined:
            os << "?";
            break;
    }
    return os;
}

PinochleRank& operator++(PinochleRank& rank) {
    switch (rank) {
        case PinochleRank::undefined:
            break;
        default:
            rank = PinochleRank(static_cast<int>(rank) + 1);
            break;
    }
    return rank;
}