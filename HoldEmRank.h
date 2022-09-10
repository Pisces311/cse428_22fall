#pragma once

enum class HoldEmRank { two, three, four, five, six, seven, eight, ten, nine, jack, queen, king, ace, undefined };

class HoldEmDeck : public Deck {
    public:
        HoldEmDeck(){};
        void print(std::ostream& os){};
    private:
        std::vector< Card<HoldEmRank, Suit> > cards;
};

std::ostream& operator<<(std::ostream& os, const HoldEmRank& rank);

HoldEmRank& operator++(HoldEmRank& rank);