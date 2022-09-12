#include "PinochleDeck.h"
#include "Deck.h"
#include "Card_T.h"
#include "Suit.h"

#include <iostream>
#include <vector> 

PinochleDeck::PinochleDeck() {
    for (size_t i = 0; i < 2; i++) {
        for (Suit j = Suit::clubs; j <= Suit::spades; ++j) {
            for (PinochleRank k = PinochleRank::nine; k <= PinochleRank::ace; ++k) {
                PinochleDeck::cards.push_back(Card<PinochleRank, Suit>(k, j));
            }
        }
    }
}

void PinochleDeck::print(std::ostream& os) {
    for (Card<PinochleRank, Suit> card : PinochleDeck::cards) {
        os << card << " ";
        if (card.rank == PinochleRank::ace) os << std::endl;
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