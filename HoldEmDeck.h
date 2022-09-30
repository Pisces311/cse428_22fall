#pragma once

#include <iostream>
#include <vector>

#include "Card_T.h"
#include "Deck_T.h"
#include "Suit.h"

enum class HoldEmRank {
    two,
    three,
    four,
    five,
    six,
    seven,
    eight,
    nine,
    ten,
    jack,
    queen,
    king,
    ace,
    undefined
};

class HoldEmDeck : public Deck<HoldEmRank, Suit> {
   public:
    HoldEmDeck();
};

std::ostream& operator<<(std::ostream& os, const HoldEmRank& rank);

HoldEmRank& operator++(HoldEmRank& rank);