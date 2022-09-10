#pragma once

enum class Rank { nine, jack, queen, king, ten, ace, undefined };

class PinochleDeck : public Deck {
    public:
        PinochleDeck(){};
        void print(std::ostream& os){};
    private:
        std::vector< Card<Rank, Suit> > cards;
};

std::ostream& operator<<(std::ostream& os, const Rank& rank);

Rank& operator++(Rank& rank);