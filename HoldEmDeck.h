#pragma once
#include <iostream>
#include <vector>

#include "Card_T.h"
#include "Deck.h"
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

class HoldEmDeck : public Deck {
   public:
    HoldEmDeck();
    void print(std::ostream& os);

   private:
    std::vector< Card<HoldEmRank, Suit> > cards;
};

std::ostream& operator<<(std::ostream& os, const HoldEmRank& rank);

HoldEmRank& operator++(HoldEmRank& rank);