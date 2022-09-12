#include "Card_T.h"

#include <iostream>

template <typename R, typename S>
ostream& operator<<(ostream& os, const Card<R, S>& card) {
    os << card.rank << card.suit;
    return os;
}