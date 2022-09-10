#include "Card.h"

#include <iostream>

template <typename R, typename S>
std::ostream& operator<<(std::ostream& os, const Card<R, S>& card) {
    return os << card.rank << card.suit;
}
