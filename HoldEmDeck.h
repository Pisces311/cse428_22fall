#pragma once
#include <iostream>
#include <vector>
#include "Deck.h"
#include "Card_T.h"
#include "Suit.h"
using namespace std;


enum class HoldEmRank { two, three, four, five, six, seven, eight, ten, nine, jack, queen, king, ace, undefined };

class HoldEmDeck : public Deck {
    public:
        HoldEmDeck();
        void print(ostream&);
    private:
        vector< Card<HoldEmRank, Suit> > cards;
};

ostream& operator<<(ostream& os, const HoldEmRank& rank);

HoldEmRank& operator++(HoldEmRank& rank);