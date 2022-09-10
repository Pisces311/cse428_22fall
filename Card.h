#pragma once

template <typename R, typename S>
struct Card {
    R rank;
    S suit;

    Card(R rank, S suit) : rank(rank), suit(suit) {}
};

template <typename R, typename S>
std::ostream& operator<<(std::ostream& os, const Card<R, S>& card);