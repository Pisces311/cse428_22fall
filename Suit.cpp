#include "Suit.h"

#include <iostream>

std::ostream& operator<<(std::ostream& os, const Suit& suit) {
    switch (suit) {
        case Suit::clubs:
            os << "C";
            break;
        case Suit::diamonds:
            os << "D";
            break;
        case Suit::hearts:
            os << "H";
            break;
        case Suit::spades:
            os << "S";
            break;
        case Suit::undefined:
            os << "?";
            break;
    }
    return os;
}

Suit& operator++(Suit& suit) {
    switch (suit) {
        case Suit::undefined:
            break;
        default:
            suit = Suit(static_cast<int>(suit) + 1);
            break;
    }
    return suit;
}