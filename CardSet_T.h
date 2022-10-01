#pragma once

#include <iostream>
#include <vector>

template <typename R, typename S>
class CardSet {
   protected:
    std::vector< Card<R, S> > cards;

   public:
    void print(std::ostream& os, size_t size);
    CardSet<R, S>& operator>>(CardSet<R, S>& cardSet);
    bool is_empty();
};

#ifdef TEMPLATE_HEADERS_INCLUDE_SOURCE
#include "CardSet_T.cpp"
#endif