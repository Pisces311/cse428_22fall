#include "PinochleGame.h"

#include <algorithm>
#include <map>

unsigned int PinochleGame::points[] = {10,  20,  40,  40,  40,  60,   80,  100,
                                       150, 300, 400, 600, 800, 1000, 1500};

PinochleGame::PinochleGame(int argc, const char** argv)
    : Game(argc, argv),
      trump_suit(Suit::undefined),
      runningTally(0),
      teamBids(2, 0) {
    scores.assign(players.size(), 0);
    for (int i = startIndex; i < argc; ++i) {
        hands.push_back(cardSetType());
    }
}

void PinochleGame::deal() {
    int playerIdx = (dealer + 1) % players.size();
    int cardCnt = 0;
    Suit lastSuit = Suit::undefined;
    while (!deck.is_empty()) {
        deck >> hands[playerIdx];

        std::vector<cardType> cardSetType::*ptr = cardSetType::getCards();
        lastSuit = (hands[playerIdx].*ptr).back().suit;

        if (++cardCnt == numInEachPacket) {
            cardCnt = 0;
            playerIdx = (playerIdx + 1) % players.size();
        }
    }
    trump_suit = lastSuit;
}

void PinochleGame::printHands() {
    for (size_t i = 0; i < hands.size(); i++) {
        std::cout << players[i];
        if (i == dealer) {
            std::cout << "*";
        }
        std::cout << "'s hand:" << std::endl;
        hands[i].print(std::cout, sizeInEachPacket);
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

int PinochleGame::computeMelds(const cardSetType& hand) {
    int score = 0;
    std::vector<PinochleMelds> melds;
    suit_independent_evaluation(hand, melds);
    suit_dependent_evaluation(hand, melds, trump_suit);
    for (PinochleMelds meld : melds) {
        score += points[static_cast<int>(meld)];
    }
    return score;
}

int PinochleGame::computeCards(const cardSetType& hand) {
    int score = 0;
    std::vector<cardType> cardSetType::*ptr = cardSetType::getCards();
    for (const cardType& card : hand.*ptr) {
        if (card.rank == PinochleRank::ten) {
            score += 10;
        } else if (card.rank == PinochleRank::ace) {
            score += 11;
        } else if (card.rank == PinochleRank::king) {
            score += 4;
        } else if (card.rank == PinochleRank::queen) {
            score += 3;
        } else if (card.rank == PinochleRank::jack) {
            score += 2;
        }
    }
    return score;
}

bool PinochleGame::bidding(std::vector<unsigned int>& bids) {
    int playerIdx = (dealer + 1) % players.size();
    int rounds = players.size();
    while (rounds--) {
        bids[playerIdx] =
            computeMelds(hands[playerIdx]) + computeCards(hands[playerIdx]);
        playerIdx = (playerIdx + 1) % players.size();
    }
    for (size_t i = 0; i < bids.size(); i++) {
        std::cout << players[i];
        if (i == dealer) {
            std::cout << "*";
        }
        std::cout << "'s bid: " << bids[i] << std::endl;
    }

    teamBids[0] = bids[0] + bids[2];
    teamBids[1] = bids[1] + bids[3];

    std::cout << "Team 1 bids: " << teamBids[0] << std::endl;
    std::cout << "Team 2 bids: " << teamBids[1] << std::endl;

    if (teamBids[0] == teamBids[1]) {
        return false;
    } else if (teamBids[0] > teamBids[1]) {
        std::cout << "Team 1 won the bid." << std::endl;
        contractTeam = 0;
        firstPlayer = bids[0] >= bids[2] ? 0 : 2;
        runningTally = computeMelds(hands[0]) + computeMelds(hands[2]);
    } else {
        std::cout << "Team 2 won the bid." << std::endl;
        contractTeam = 1;
        firstPlayer = bids[1] >= bids[3] ? 1 : 3;
        runningTally = computeMelds(hands[1]) + computeMelds(hands[3]);
    }
    std::cout << std::endl;
    return true;
}

Suit PinochleGame::playFirstCard(std::vector<cardType>& trickCards,
                                 std::vector<cardType>& cards) {
    Suit retSuit;
    PinochleRank highestRank = PinochleRank::nine;
    for (const cardType& card : cards) {
        if (card.rank > highestRank) {
            retSuit = card.suit;
            highestRank = card.rank;
        } else if (card.rank == highestRank && retSuit != trump_suit) {
            if (card.suit == trump_suit || card.suit > retSuit) {
                retSuit = card.suit;
            }
        }
    }

    for (auto it = cards.begin(); it != cards.end(); it++) {
        if (it->suit == retSuit && it->rank == highestRank) {
            cards.erase(it);
            break;
        }
    }

    trickCards.emplace_back(highestRank, retSuit);

    return retSuit;
}

// play the highest card of the given suit, if the rank is higher than every
// card in the trick of the given suit
bool PinochleGame::playHighestCard(std::vector<cardType>& trickCards,
                                   std::vector<cardType>& cards, Suit suit) {
    PinochleRank highestRank = PinochleRank::nine;
    for (const cardType& card : cards) {
        if (card.suit == suit && card.rank > highestRank) {
            highestRank = card.rank;
        }
    }

    for (const cardType& card : trickCards) {
        if (card.suit == suit && card.rank > highestRank) {
            return false;
        }
    }

    for (auto it = cards.begin(); it != cards.end(); it++) {
        if (it->suit == suit && it->rank == highestRank) {
            cards.erase(it);
            break;
        }
    }

    trickCards.emplace_back(highestRank, suit);

    return true;
}

// play the lowest card of the given suit
void PinochleGame::playLowestCard(std::vector<cardType>& trickCards,
                                  std::vector<cardType>& cards, Suit suit) {
    PinochleRank lowestRank = PinochleRank::ace;
    for (const cardType& card : cards) {
        if (card.suit == suit && card.rank < lowestRank) {
            lowestRank = card.rank;
        }
    }

    for (auto it = cards.begin(); it != cards.end(); it++) {
        if (it->suit == suit && it->rank == lowestRank) {
            cards.erase(it);
            break;
        }
    }

    trickCards.emplace_back(lowestRank, suit);
}

void PinochleGame::playAnyLowestCard(std::vector<cardType>& trickCards,
                                     std::vector<cardType>& cards) {
    PinochleRank lowestRank = PinochleRank::ace;

    for (const cardType& card : cards) {
        if (card.rank < lowestRank && card.suit != trump_suit) {
            lowestRank = card.rank;
        }
    }

    Suit lowestSuit = Suit::undefined;
    for (auto it = cards.begin(); it != cards.end(); it++) {
        if (it->rank == lowestRank && it->suit != trump_suit) {
            lowestSuit = it->suit;
            cards.erase(it);
            break;
        }
    }

    trickCards.emplace_back(lowestRank, lowestSuit);
}

// return true if the card1 is higher than card2
bool PinochleGame::compareCards(const cardType& card1, const cardType& card2,
                                Suit leadingSuit) {
    if (card1.suit == trump_suit && card2.suit == trump_suit) {
        return card1.rank > card2.rank;
    } else if (card1.suit == trump_suit) {
        return true;
    } else if (card2.suit == trump_suit) {
        return false;
    } else if (card1.suit == leadingSuit && card2.suit == leadingSuit) {
        return card1.rank > card2.rank;
    } else if (card1.suit == leadingSuit) {
        return true;
    } else if (card2.suit == leadingSuit) {
        return false;
    } else {
        return card1.rank > card2.rank;
    }
}

void PinochleGame::printTrick(const cardSetType& trick) {
    std::vector<cardType> cardSetType::*ptr = cardSetType::getCards();
    std::cout << "Trick: ";
    for (const cardType& card : trick.*ptr) {
        std::cout << card << " ";
    }
    std::cout << std::endl;
}

// return the winner of this round
int PinochleGame::playTrick(cardSetType& trick) {
    std::vector<cardType> cardSetType::*ptr = cardSetType::getCards();

    // play the first card
    Suit leadingSuit = playFirstCard(trick.*ptr, hands[firstPlayer].*ptr);

    std::cout << "player " << players[firstPlayer];
    if (firstPlayer == dealer) std::cout << "*";
    std::cout << " played " << (trick.*ptr).back() << std::endl;

    size_t playerIdx = (firstPlayer + 1) % players.size();
    do {
        if (leadingSuit == trump_suit) {
            if (hasSuit(hands[playerIdx].*ptr, trump_suit)) {
                if (!playHighestCard(trick.*ptr, hands[playerIdx].*ptr,
                                     trump_suit)) {
                    playLowestCard(trick.*ptr, hands[playerIdx].*ptr,
                                   trump_suit);
                }
            } else {
                playAnyLowestCard(trick.*ptr, hands[playerIdx].*ptr);
            }
        } else {
            if (hasSuit(hands[playerIdx].*ptr, leadingSuit)) {
                if (hasSuit(trick.*ptr, trump_suit)) {
                    playLowestCard(trick.*ptr, hands[playerIdx].*ptr,
                                   leadingSuit);
                } else if (!playHighestCard(trick.*ptr, hands[playerIdx].*ptr,
                                            leadingSuit)) {
                    playLowestCard(trick.*ptr, hands[playerIdx].*ptr,
                                   leadingSuit);
                }
            } else {
                if (!playHighestCard(trick.*ptr, hands[playerIdx].*ptr,
                                     trump_suit)) {
                    playAnyLowestCard(trick.*ptr, hands[playerIdx].*ptr);
                }
            }
        }

        std::cout << "player " << players[playerIdx];
        if (playerIdx == dealer) std::cout << "*";
        std::cout << " played " << (trick.*ptr).back() << std::endl;

        printTrick(trick);

        playerIdx = (playerIdx + 1) % players.size();
    } while (playerIdx != firstPlayer);

    // find the winner
    int highestCardIdx = 0;
    for (size_t i = 1; i < (trick.*ptr).size(); i++) {
        if (compareCards((trick.*ptr)[i], (trick.*ptr)[highestCardIdx],
                         leadingSuit)) {
            highestCardIdx = i;
        }
    }

    size_t winnerIdx = (firstPlayer + highestCardIdx) % players.size();
    std::cout << "player " << players[winnerIdx];
    if (winnerIdx == dealer) std::cout << "*";
    std::cout << " won the trick" << std::endl;

    return winnerIdx;
}

void PinochleGame::addScore(int winnerTeam, const cardSetType& trick) {
    if (winnerTeam == contractTeam) {
        runningTally += computeCards(trick);
    }
}

void PinochleGame::playTricks() {
    int rounds = 12;
    cardSetType trick;
    while (rounds--) {
        std::cout << "Round " << 12 - rounds << std::endl;
        std::cout << "---------------" << std::endl;
        int winner = playTrick(trick);
        int winnerTeam = winner % 2;
        addScore(winnerTeam, trick);
        deck.collect(trick);
        firstPlayer = winner;
        if (!rounds && winnerTeam == contractTeam) {
            runningTally += 10;
        }
        std::cout << std::endl;

        printHands();
    }

    if (runningTally >= teamBids[contractTeam]) {
        std::cout << "Contract succeeded!" << std::endl;
        scores[contractTeam] += runningTally;
    } else {
        std::cout << "Contract failed!" << std::endl;
    }

    std::cout << "Team 1 score: " << scores[0] << std::endl;
    std::cout << "Team 2 score: " << scores[1] << std::endl;
}

int PinochleGame::hasWinner() {
    if (scores[0] >= 1500) return 1;
    if (scores[1] >= 1500) return 2;
    return 0;
}

int PinochleGame::play() {
    while (true) {
        deck.shuffle();
        deal();
        printHands();
        std::cout << "Trump suit: " << trump_suit << std::endl;
        std::vector<unsigned int> bids(players.size());
        if (!bidding(bids)) {
            std::cout << "Bidding failed. Re-dealing." << std::endl;
            collectHands();
            continue;
        }
        // play tricks
        playTricks();
        int winner = hasWinner();
        if (winner) {
            std::cout << "Team " << winner << " won!" << std::endl;
            break;
        }
        if (continuePrompt()) {
            break;
        }
        dealer = (dealer + 1) % players.size();
    }
    return SUCCESS;
}

bool PinochleGame::hasSuit(const std::vector<cardType>& cards, Suit suit) {
    for (const cardType& card : cards) {
        if (card.suit == suit) {
            return true;
        }
    }
    return false;
}

int PinochleGame::hasSets(const std::vector<cardType>& cards,
                          PinochleRank rank) {
    std::vector<int> suitCount(4);
    for (const cardType& card : cards) {
        if (card.rank == rank) {
            suitCount[static_cast<int>(card.suit)]++;
        }
    }
    return *min_element(suitCount.begin(), suitCount.end());
}

void PinochleGame::suit_independent_evaluation(
    const cardSetType& hand, std::vector<PinochleMelds>& melds) {
    cardSetType handCopy = hand;
    std::vector<cardType> cardSetType::*cards = cardSetType::getCards();

    // thousandaces & hundredaces
    switch (hasSets(handCopy.*cards, PinochleRank::ace)) {
        case 2:
            melds.push_back(PinochleMelds::thousandaces);
            break;
        case 1:
            melds.push_back(PinochleMelds::hundredaces);
            break;
    }

    // eighthundredkings & eightykings
    switch (hasSets(handCopy.*cards, PinochleRank::king)) {
        case 2:
            melds.push_back(PinochleMelds::eighthundredkings);
            break;
        case 1:
            melds.push_back(PinochleMelds::eightykings);
            break;
    }

    // sixhundredqueens & sixtyqueens
    switch (hasSets(handCopy.*cards, PinochleRank::queen)) {
        case 2:
            melds.push_back(PinochleMelds::sixhundredqueens);
            break;
        case 1:
            melds.push_back(PinochleMelds::sixtyqueens);
            break;
    }

    // fourhundredjacks & fortyjacks
    switch (hasSets(handCopy.*cards, PinochleRank::jack)) {
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

bool PinochleGame::isInsuitdoublerun(const std::vector<cardType>& cards,
                                     Suit suit) {
    std::map<PinochleRank, int> rankCount;
    std::vector<PinochleRank> deck = {PinochleRank::ten, PinochleRank::jack,
                                      PinochleRank::queen, PinochleRank::king,
                                      PinochleRank::ace};
    for (const cardType& card : cards) {
        if (card.suit == suit && card.rank != PinochleRank::nine) {
            rankCount[card.rank]++;
        }
    }
    for (PinochleRank rank : deck) {
        if (rankCount[rank] < 2) {
            return false;
        }
    }
    return true;
}

bool PinochleGame::isInsuitrun(const std::vector<cardType>& cards, Suit suit) {
    std::map<PinochleRank, int> rankCount;
    std::vector<PinochleRank> deck = {PinochleRank::ten, PinochleRank::jack,
                                      PinochleRank::queen, PinochleRank::king,
                                      PinochleRank::ace};
    for (const cardType& card : cards) {
        if (card.suit == suit && card.rank != PinochleRank::nine) {
            rankCount[card.rank]++;
        }
    }
    for (PinochleRank rank : deck) {
        if (rankCount[rank] < 1) {
            return false;
        }
    }
    return true;
}

bool PinochleGame::isInsuitmarriage(const std::vector<cardType>& cards,
                                    Suit suit) {
    bool hasQueen = false, hasKing = false;
    for (const cardType& card : cards) {
        if (card.suit == suit) {
            if (card.rank == PinochleRank::queen) {
                hasQueen = true;
            } else if (card.rank == PinochleRank::king) {
                hasKing = true;
            }
        }
    }
    return hasQueen && hasKing;
}

bool PinochleGame::isOffsuitmarriage(const std::vector<cardType>& cards,
                                     Suit suit) {
    for (Suit s = Suit::clubs; s <= Suit::spades; ++s) {
        if (s != suit && isInsuitmarriage(cards, s)) {
            return true;
        }
    }
    return false;
}

bool PinochleGame::isDix(const std::vector<cardType>& cards, Suit suit) {
    for (const cardType& card : cards) {
        if (card.suit == suit && card.rank == PinochleRank::nine) {
            return true;
        }
    }
    return false;
}

void PinochleGame::suit_dependent_evaluation(const cardSetType& hand,
                                             std::vector<PinochleMelds>& melds,
                                             Suit suit) {
    cardSetType handCopy = hand;
    std::vector<cardType> cardSetType::*cards = cardSetType::getCards();

    // insuitdoublerun & insuitrun
    if (isInsuitdoublerun(handCopy.*cards, suit)) {
        melds.push_back(PinochleMelds::insuitdoublerun);
    } else if (isInsuitrun(handCopy.*cards, suit)) {
        melds.push_back(PinochleMelds::insuitrun);
    }

    // insuitmarriage & offsuitmarriage
    if (isInsuitmarriage(handCopy.*cards, suit)) {
        melds.push_back(PinochleMelds::insuitmarriage);
    } else if (isOffsuitmarriage(handCopy.*cards, suit)) {
        melds.push_back(PinochleMelds::offsuitmarriage);
    }

    // dix
    if (isDix(handCopy.*cards, suit)) {
        melds.push_back(PinochleMelds::dix);
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