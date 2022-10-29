#include "PinochleGame.h"

unsigned int PinochleGame::points[] = {10,  20,  40,  40,  40,  60,   80,  100,
                                       150, 300, 400, 600, 800, 1000, 1500};

PinochleGame::PinochleGame(int argc, const char** argv) : Game(argc, argv) {
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

void PinochleGame::suit_independent_evaluation(
    const CardSet<PinochleRank, Suit>& hand,
    std::vector<PinochleMelds>& melds) {
    // TODO: implement
}

std::ostream& operator<<(std::ostream& os, const PinochleMelds& meld) {
    switch (meld) {
        case PinochleMelds::dix:
            os << "Dix";
            break;
        case PinochleMelds::offsuitmarriage:
            os << "Offsuit Marriage";
            break;
        case PinochleMelds::fortyjacks:
            os << "Forty Jacks";
            break;
        case PinochleMelds::pinochle:
            os << "Pinochle";
            break;
        case PinochleMelds::insuitmarriage:
            os << "Insuit Marriage";
            break;
        case PinochleMelds::sixtyqueens:
            os << "Sixty Queens";
            break;
        case PinochleMelds::eightykings:
            os << "Eighty Kings";
            break;
        case PinochleMelds::hundredaces:
            os << "Hundred Aces";
            break;
        case PinochleMelds::insuitrun:
            os << "Insuit Run";
            break;
        case PinochleMelds::doublepinochle:
            os << "Double Pinochle";
            break;
        case PinochleMelds::fourhundredjacks:
            os << "Four Hundred Jacks";
            break;
        case PinochleMelds::sixhundredqueens:
            os << "Six Hundred Queens";
            break;
        case PinochleMelds::eighthundredkings:
            os << "Eight Hundred Kings";
            break;
        case PinochleMelds::thousandaces:
            os << "Thousand Aces";
            break;
        case PinochleMelds::insuitdoublerun:
            os << "Insuit Double Run";
            break;
    }
    os << ' ' << PinochleGame::points[static_cast<int>(meld)];
    return os;
}