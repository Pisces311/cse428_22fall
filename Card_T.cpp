#include "Card_T.h"

#include <iostream>

template <typename R, typename S>
std::ostream& operator<<(std::ostream& os, const Card<R, S>& card) {
    os << card.rank << card.suit;
    return os;
}

template <typename R, typename S>
bool compareByRank(const Card<R, S>& lhs, const Card<R, S>& rhs) {
    return lhs.rank == rhs.rank ? lhs.suit < rhs.suit : lhs.rank < rhs.rank;
}

template <typename R, typename S>
bool compareBySuit(const Card<R, S>& lhs, const Card<R, S>& rhs) {
    return lhs.suit == rhs.suit ? lhs.rank < rhs.rank : lhs.suit < rhs.suit;
}