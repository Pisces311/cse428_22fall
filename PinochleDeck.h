#pragma once
#include <iostream>
#include <vector>

#include "Card_T.h"
#include "Deck_T.h"
#include "Suit.h"

enum class PinochleRank { nine, jack, queen, king, ten, ace, undefined };

class PinochleDeck : public Deck<PinochleRank, Suit> {
    const size_t numDecks = 2;

   public:
    PinochleDeck();
};

std::ostream& operator<<(std::ostream& os, const PinochleRank& rank);

PinochleRank& operator++(PinochleRank& rank);