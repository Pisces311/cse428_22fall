#include "HoldEmDeck.h"
#include "PinochleDeck.h"

const int SUCCESS = 0;

int main() {
    HoldEmDeck holdEmDeck;
    PinochleDeck pinochleDeck;

    holdEmDeck.shuffle();
    holdEmDeck.print(std::cout, 13);
    // pinochleDeck.print(std::cout, 6);

    return SUCCESS;
}