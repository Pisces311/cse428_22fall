#include "PinochleGame.h"

PinochleGame::PinochleGame(int argc, const char **argv) : Game(argc, argv) {
    for (int i = startIndex; i < argc; ++i) {
        hands.push_back(CardSet<PinochleRank, Suit>());
    }
}

void PinochleGame::deal() {
    int playerIdx = 0, cardCnt = 0;
    while (!deck.is_empty()) {
        deck >> hands[playerIdx];
        if (++cardCnt == 3) {
            cardCnt = 0;
            playerIdx = (playerIdx + 1) % hands.size();
        }
    }
}

void PinochleGame::printHands() {
    for (size_t i = 0; i < hands.size(); i++) {
        std::cout << players[i] << "'s hand:" << std::endl;
        hands[i].print(std::cout, 3);
    }
}

void PinochleGame::collectHands() {
    for (size_t i = 0; i < hands.size(); i++) {
        deck.collect(hands[i]);
    }
}

int PinochleGame::play() {
    while (true) {
        deck.shuffle();
        deal();
        printHands();
        collectHands();
        if (continuePrompt()) {
            break;
        }
    }
    return SUCCESS;
}