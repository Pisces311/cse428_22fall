#include "HoldEmDeck.h"
#include "PinochleDeck.h"

const int SUCCESS = 0;

int main() {
    HoldEmDeck holdEmDeck;
    PinochleDeck pinochleDeck;

    holdEmDeck.print(std::cout);
    pinochleDeck.print(std::cout);

    return SUCCESS;
}