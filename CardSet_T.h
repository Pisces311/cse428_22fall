#pragma once

#include <iostream>
#include <vector>

#include "Card_T.h"

template <typename R, typename S>
class CardSet {
   protected:
    std::vector<Card<R, S> > cards;

   public:
    CardSet() = default;
    CardSet(const CardSet<R, S>& other) : cards(other.cards) {}
    CardSet& operator=(const CardSet<R, S>& other) = default;

    static std::vector<Card<R, S> > CardSet<R, S>::*getCards();

    void print(std::ostream& os, size_t size);
    CardSet<R, S>& operator>>(CardSet<R, S>& cardSet);
    bool is_empty();
};

#ifdef TEMPLATE_HEADERS_INCLUDE_SOURCE
#include "CardSet_T.cpp"
#endif