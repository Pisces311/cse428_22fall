#include "HoldEmGame.h"

HoldEmGame::HoldEmGame(int argc, const char **argv) : Game(argc, argv) {
    state = HoldEmState::preflop;
    for (int i = startIndex; i < argc; ++i) {
        hands.push_back(CardSet<HoldEmRank, Suit>());
    }
}

void HoldEmGame::deal() {
    switch (state) {
        case HoldEmState::preflop:
            for (size_t i = 0; i < 2; i++) {
                for (size_t j = 0; j < hands.size(); j++) {
                    deck >> hands[j];
                }
            }
            state = HoldEmState(static_cast<int>(state) + 1);
            break;
        case HoldEmState::flop:
            for (size_t i = 0; i < 3; i++) {
                    deck >> commonBoards;
                }
            state = HoldEmState(static_cast<int>(state) + 1);
            break;
        case HoldEmState::turn:
        case HoldEmState::river:
            deck >> commonBoards;
            state = HoldEmState(static_cast<int>(state) + 1);
            break;
        default:
            break;
    }
}

void HoldEmGame::printHands() {
    for (size_t i = 0; i < hands.size(); i++) {
        std::cout << players[i] << "'s hand:" << std::endl;
        hands[i].print(std::cout, 2);
    }
}

void HoldEmGame::printBoards() {
    commonBoards.print(std::cout, 3);
}

void HoldEmGame::collectHands() {
    for (size_t i = 0; i < hands.size(); i++) {
        deck.collect(hands[i]);
    }
}

void HoldEmGame::collectBoards() {
    deck.collect(commonBoards);
}

int HoldEmGame::play() {
    while (true) {
        deck.shuffle();
        state = HoldEmState::preflop;
        deal();
        printHands();

        deal();
        std::cout << "BOARD(flop): " << std::endl;
        printBoards();

        deal();
        std::cout << "BOARD(turn): " << std::endl;
        printBoards();

        deal();
        std::cout << "BOARD(river): " << std::endl;
        printBoards();

        collectHands();
        collectBoards();

        if (continuePrompt()) {
            break;
        }
    }
    return SUCCESS;
}