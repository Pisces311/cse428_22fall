#include "PinochleRank.h"
#include "Deck.h"
#include "Card.h"
#include "Suit.h"

#include <iostream>
#include <vector> 

PinochleDeck::PinochleDeck() {
    for (size_t i = 0; i < 2; i++) {
        for (Suit j = Suit::clubs; j <= Suit::spades; ++j) {
            for (Rank k = Rank::nine; k < Rank::ace; ++k) {
                PinochleDeck::cards.push_back(Card<Rank, Suit>(k, j));
            }
        }
    }
}

void PinochleDeck::print(std::ostream& os) {
    for (Card<Rank, Suit> card : PinochleDeck::cards) {
        os << card << " ";
        if (card.rank == Rank::ace) os << std::endl;
    }
}

std::ostream& operator<<(std::ostream& os, const Rank& rank) {
    switch (rank) {
        case Rank::nine:
            os << "9";
            break;
        case Rank::jack:
            os << "J";
            break;   
        case Rank::queen:
            os << "Q";
            break;
        case Rank::king:
            os << "K";
            break;
        case Rank::ten:
            os << "10";
            break;
        case Rank::ace:
            os << "A";
            break;
        case Rank::undefined:
            os << "?";
            break;
    }
    return os;
}

Rank& operator++(Rank& rank) {
    switch (rank) {
        case Rank::undefined:
            break;
        default:
            rank = Rank(static_cast<int>(rank) + 1);
            break;
    }
}