#include "HoldEmGame.h"

HoldEmGame::HoldEmGame(int argc, const char **argv) : Game(argc, argv) {
    for (int i = startIndex; i < argc; ++i) {
        hands.push_back(CardSet<HoldEmRank, Suit>());
    }
}

void HoldEmGame::printHands() {
    for (size_t i = 0; i < hands.size(); i++) {
        std::cout << players[i] << "'s hand:" << std::endl;
        hands[i].print(std::cout, 3);
    }
}

void HoldEmGame::collectHands() {
    for (size_t i = 0; i < hands.size(); i++) {
        deck.collect(hands[i]);
    }
}