#include "CardSet_T.h"

#include <iostream>

template <typename R, typename S>
void CardSet<R, S>::print(std::ostream& os, size_t size) {
    size_t cnt = 0;
    for (typename std::vector<Card<R, S> >::iterator it = cards.begin();
         it != cards.end(); ++it) {
        os << *it << ' ';
        if (++cnt % size == 0 || cnt == cards.size()) {
            os << std::endl;
        }
    }
}