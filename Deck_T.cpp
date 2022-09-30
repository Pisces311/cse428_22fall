#include "Deck_T.h"

#include <algorithm>
#include <random>

template <typename R, typename S>
void Deck<R, S>::shuffle() {
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(CardSet<R, S>::cards.begin(), CardSet<R, S>::cards.end(), g);
}