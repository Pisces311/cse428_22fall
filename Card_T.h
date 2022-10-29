#pragma once

template <typename R, typename S>
struct Card {
    R rank;
    S suit;

    Card(R rank, S suit) : rank(rank), suit(suit) {}
};

template <typename R, typename S>
std::ostream& operator<<(std::ostream& os, const Card<R, S>& card);

template <typename R, typename S>
bool compareByRank(const Card<R, S>& lhs, const Card<R, S>& rhs);

template <typename R, typename S>
bool compareBySuit(const Card<R, S>& lhs, const Card<R, S>& rhs);

#ifdef TEMPLATE_HEADERS_INCLUDE_SOURCE
#include "Card_T.cpp"
#endif