#pragma once
#include <iostream>
#include <vector>

#include "Card_T.h"
#include "Deck.h"
#include "Suit.h"

enum class PinochleRank { nine, jack, queen, king, ten, ace, undefined };

class PinochleDeck : public Deck {
   public:
    PinochleDeck();
    void print(std::ostream& os);

   private:
    std::vector< Card<PinochleRank, Suit> > cards;
};

std::ostream& operator<<(std::ostream& os, const PinochleRank& rank);

PinochleRank& operator++(PinochleRank& rank);