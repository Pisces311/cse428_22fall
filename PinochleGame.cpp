#include "PinochleGame.h"

#include <algorithm>

unsigned int PinochleGame::points[] = {10,  20,  40,  40,  40,  60,   80,  100,
                                       150, 300, 400, 600, 800, 1000, 1500};

PinochleGame::PinochleGame(int argc, const char** argv) : Game(argc, argv) {
    for (int i = startIndex; i < argc; ++i) {
        hands.push_back(cardSetType());
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
        std::vector<PinochleMelds> melds;
        suit_independent_evaluation(hands[i], melds);
        std::cout << "Melds:" << std::endl;
        for (auto meld : melds) {
            std::cout << meld << std::endl;
        }
        std::cout << std::endl;
    }
}

void PinochleGame::collectHands() {
    for (cardSetType& hand : hands) {
        deck.collect(hand);
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
    const cardSetType& hand, std::vector<PinochleMelds>& melds) {
    cardSetType handCopy = hand;
    std::vector<cardType> cardSetType::*cards = cardSetType::getCards();

    auto hasSets = [&](PinochleRank rank) {
        std::vector<int> suitCount(4);
        for (const cardType& card : handCopy.*cards) {
            if (card.rank == rank) {
                suitCount[static_cast<int>(card.suit)]++;
            }
        }
        return *min_element(suitCount.begin(), suitCount.end());
    };

    // thousandaces & hundredaces
    switch (hasSets(PinochleRank::ace)) {
        case 2:
            melds.push_back(PinochleMelds::thousandaces);
            break;
        case 1:
            melds.push_back(PinochleMelds::hundredaces);
            break;
    }

    // eighthundredkings & eightykings
    switch (hasSets(PinochleRank::king)) {
        case 2:
            melds.push_back(PinochleMelds::eighthundredkings);
            break;
        case 1:
            melds.push_back(PinochleMelds::eightykings);
            break;
    }

    // sixhundredqueens & sixtyqueens
    switch (hasSets(PinochleRank::queen)) {
        case 2:
            melds.push_back(PinochleMelds::sixhundredqueens);
            break;
        case 1:
            melds.push_back(PinochleMelds::sixtyqueens);
            break;
    }

    // fourhundredjacks & fortyjacks
    switch (hasSets(PinochleRank::jack)) {
        case 2:
            melds.push_back(PinochleMelds::fourhundredjacks);
            break;
        case 1:
            melds.push_back(PinochleMelds::fortyjacks);
            break;
    }

    // doublepinochle & pinochle
    int JDiamonds = 0, QSpades = 0;
    for (const cardType& card : handCopy.*cards) {
        if (card.rank == PinochleRank::jack && card.suit == Suit::diamonds) {
            JDiamonds++;
        } else if (card.rank == PinochleRank::queen &&
                   card.suit == Suit::spades) {
            QSpades++;
        }
    }
    switch (std::min(JDiamonds, QSpades)) {
        case 2:
            melds.push_back(PinochleMelds::doublepinochle);
            break;
        case 1:
            melds.push_back(PinochleMelds::pinochle);
            break;
    }
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