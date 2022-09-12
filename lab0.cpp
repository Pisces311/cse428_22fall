#include "HoldEmDeck.h"
#include "PinochleDeck.h"

#include <stack>

int main() {
    std::stack<HoldEmDeck> holdEmStack;
    std::stack<PinochleDeck> PinochleStack;

    HoldEmDeck* holdEmDeck = new HoldEmDeck();
    PinochleDeck* pinochleDeck = new PinochleDeck();

    holdEmDeck->print(std::cout);
    pinochleDeck->print(std::cout);

    return 0;
}