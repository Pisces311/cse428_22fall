#pragma once

#include <iostream>

#include "CardSet_T.h"

template <typename R, typename S>
class Deck : public CardSet<R, S> {
   public:
    void shuffle();
    virtual ~Deck() = default;
};

#ifdef TEMPLATE_HEADERS_INCLUDE_SOURCE
#include "Deck_T.cpp"
#endif