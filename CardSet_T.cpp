#include "CardSet_T.h"

#include <iostream>

#include "Card_T.h"

template <typename R, typename S>
std::vector<Card<R, S> > CardSet<R, S>::*getCards() {
    return &CardSet<R, S>::cards;
}

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

template <typename R, typename S>
CardSet<R, S>& CardSet<R, S>::operator>>(CardSet<R, S>& cardSet) {
    if (is_empty()) {
        throw std::runtime_error("Empty cardset\n");
    }
    Card<R, S> card = CardSet<R, S>::cards.back();
    CardSet<R, S>::cards.pop_back();
    cardSet.cards.push_back(card);
    return *this;
}

template <typename R, typename S>
bool CardSet<R, S>::is_empty() {
    return CardSet<R, S>::cards.empty();
}