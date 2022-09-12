#pragma once

template <typename R, typename S>
struct Card {
    R rank;
    S suit;

    Card(R rank, S suit) : rank(rank), suit(suit) {}
};

template <typename R, typename S>
std::ostream& operator<<(std::ostream& os, const Card<R, S>& card);

#ifdef TEMPLATE_HEADERS_INCLUDE_SOURCE
#include "Card_T.cpp"
#endif