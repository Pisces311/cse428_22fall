#pragma once
#include <iostream>
#include <vector>
#include "Deck.h"
#include "Card.h"
#include "Suit.h"
using namespace std;

enum class PinochleRank { nine, jack, queen, king, ten, ace, undefined };

class PinochleDeck : public Deck {
    public:
        PinochleDeck();
        void print(ostream&);
    private:
        vector< Card<PinochleRank, Suit> > cards;
};

ostream& operator<<(ostream& os, const PinochleRank& rank);

PinochleRank& operator++(PinochleRank& rank);