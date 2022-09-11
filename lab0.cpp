#include "HoldEmDeck.h"
#include "PinochleDeck.h"

#include <stack>

int main(int agrc, char* argv[]) {
    std::stack<HoldEmDeck> holdEmStack;
    std::stack<PinochleDeck> PinochleStack;

    HoldEmDeck holdEmDeck = HoldEmDeck();
    PinochleDeck pinochleDeck = PinochleDeck();

    holdEmDeck.print(std::cout);
    pinochleDeck.print(std::cout);

    return 0;
}