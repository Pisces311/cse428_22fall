#pragma once
using namespace std;

template <typename R, typename S>
struct Card {
    R rank;
    S suit;

    Card(R rank, S suit) : rank(rank), suit(suit) {}
};

template <typename R, typename S>
ostream& operator<<(ostream& os, const Card<R, S>& card);