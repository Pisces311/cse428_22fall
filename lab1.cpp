#include "PinochleGame.h"

const int SUCCESS = 0;

int main(int argc, const char **argv) {
    // HoldEmDeck holdEmDeck;
    // PinochleDeck pinochleDeck1;
    // PinochleDeck pinochleDeck2;

//    exercise 9 test
//    holdEmDeck.shuffle();
//    holdEmDeck.print(std::cout, 13);
//     pinochleDeck.print(std::cout, 6);

//    exercise 9 test
//    pinochleDeck1.print(std::cout, 10);
//    pinochleDeck2.print(std::cout, 10);
//    pinochleDeck1 >> pinochleDeck2;
//    pinochleDeck1.print(std::cout, 10);
//    pinochleDeck2.print(std::cout, 10);

//    exercise 9 test
//    pinochleDeck1.print(std::cout, 10);
//    pinochleDeck2.print(std::cout, 10);
//    pinochleDeck1.collect(pinochleDeck2);
//    pinochleDeck1.print(std::cout, 10);
//    pinochleDeck2.print(std::cout, 10);

    PinochleGame game(argc, argv);
    game.play();

    return SUCCESS;
}