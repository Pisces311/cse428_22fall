#include "HoldEmGame.h"

#include <algorithm>

HoldEmGame::HoldEmGame(int argc, const char **argv) : Game(argc, argv) {
    state = HoldEmState::preflop;
    for (int i = startIndex; i < argc; ++i) {
        hands.push_back(cardSetType());
    }
    scores.assign(players.size(), initialScore);
}

void HoldEmGame::deal() {
    int cardToSend = numPreflopRound * players.size();
    int playerIdx = (dealer + 1) % players.size();
    switch (state) {
        case HoldEmState::preflop:
            while (cardToSend--) {
                deck >> hands[playerIdx];
                playerIdx = (playerIdx + 1) % players.size();
            }
            state = HoldEmState(static_cast<int>(state) + 1);
            break;
        case HoldEmState::flop:
            for (size_t i = 0; i < numCommonBoardsBatch; i++) {
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
        std::cout << players[i];
        if (i == dealer) {
            std::cout << "*";
        }
        std::cout << "'s hand:" << std::endl;
        hands[i].print(std::cout, numPlayerHands);
    }
    std::cout << "" << std::endl;
}

void HoldEmGame::printBoards() {
    commonBoards.print(std::cout, numCommonBoardsBatch);
}

void HoldEmGame::printSortedHands() {
    structs.clear();
    for (size_t i = 0; i < players.size(); i++) {
        structs.emplace_back(hands[i], players[i], HoldEmHandRank::undefined,
                             i);
    }
    std::vector<cardType> cardSetType::*cards = cardSetType::getCards();
    for (size_t i = 0; i < structs.size(); i++) {
        for (size_t j = 0; j < (commonBoards.*cards).size(); j++) {
            ((structs[i].cardSet).*cards).push_back((commonBoards.*cards)[j]);
        }
        if (((structs[i].cardSet).*cards).size() >= numCardsAfterTurn) {
            structs[i] = findBestHand(structs[i].cardSet, structs[i].playerName, structs[i].playerIdx);
        }
        else {
            structs[i].rank = holdem_hand_evaluation(structs[i].cardSet);
        }
    }

    // sort by player's cardset rank
    std::vector<HoldEmGameStruct> sortedStructs = structs;
    std::sort(sortedStructs.begin(), sortedStructs.end(), operator<);
    std::reverse(sortedStructs.begin(), sortedStructs.end());
    std::cout << "" << std::endl;
    for (size_t i = 0; i < sortedStructs.size(); i++) {
        std::cout << sortedStructs[i].playerName;
        if (sortedStructs[i].playerIdx == dealer) {
            std::cout << "*";
        }
        std::cout << ": ";
        for (size_t j = 0; j < ((sortedStructs[i].cardSet).*cards).size(); j++) {
            std::cout << ((sortedStructs[i].cardSet).*cards)[j] << " ";
        }
        std::cout << "(" << sortedStructs[i].rank << ")" << std::endl;
    }
    std::cout << "" << std::endl;
}

HoldEmGame::HoldEmGameStruct HoldEmGame::findBestHand(const cardSetType &hand, 
        std::string &givenPlayerName, size_t playerIdx) {
    std::vector<cardType> cardSetType::*cards = cardSetType::getCards();
    size_t numCards = (hand.*cards).size();
    std::vector<HoldEmGameStruct> tempStructs;

    // get all combinations of hand
    if (numCards == numCardsAfterTurn) {
        for (size_t i = 0; i < numCards; i++) {
            cardSetType handCopy = hand;
            // remove 1 card from all 6 cards
            (handCopy.*cards).erase((handCopy.*cards).begin() + i);
            HoldEmHandRank rank = holdem_hand_evaluation(handCopy);
            tempStructs.emplace_back(handCopy, givenPlayerName, rank, playerIdx);
        }
    }
    else if (numCards == numCardsAfterRiver) {
        for (size_t i = 0; i < numCards-1; i++) {
            for (size_t j = i+1; j < numCards; j++) {
                cardSetType handCopy = hand;
                // remove 2 cards from all 6 cards
                (handCopy.*cards).erase((handCopy.*cards).begin() + i);
                (handCopy.*cards).erase((handCopy.*cards).begin() + (j-1));
                HoldEmHandRank rank = holdem_hand_evaluation(handCopy);
                tempStructs.emplace_back(handCopy, givenPlayerName, rank, playerIdx);
            }
        }
    }
    // get the best hand for current player
    std::sort(tempStructs.begin(), tempStructs.end(), operator<);
    HoldEmGameStruct bestHand = tempStructs.back();
    return bestHand;
}

bool HoldEmGame::bet() {
    betChips.assign(players.size(), 0);
    raiseTimes.assign(players.size(), 0);

    bool end = false;

    switch (state)
    {
    case HoldEmState::flop:
        end = preflopBet();
        break;
    case HoldEmState::turn:
    case HoldEmState::river:
    case HoldEmState::undefined:
        end = postflopBet();
        break;
    default:
        break;
    }

    addToPot();
    printRoundStats();
    return end;
}

void HoldEmGame::printRoundStats() {
    std::cout << "----------Current round stats!----------" << std::endl;
    for (size_t i = 0; i < players.size(); i++) {
        std::cout << players[i] << ": ";
        std::cout << "score: " << scores[i];
        std::cout << ", bet: " << betChips[i];
        std::cout << ", fold: " << foldState[i];
        std::cout << ", call: " << callState[i];
        std::cout << ", raise times: " << raiseTimes[i] << std::endl;
    }
    std::cout << "current pot: " << commonPot << std::endl;
    std::cout << "" << std::endl;
}

void HoldEmGame::addToPot() {
    for (size_t i = 0; i < players.size(); i++)
    {
        commonPot += betChips[i];
    }
}

void HoldEmGame::processChips(const int &currPlayerIdx, const unsigned int &diffBet) {
    if (scores[currPlayerIdx] < diffBet) {
        betChips[currPlayerIdx] += scores[currPlayerIdx];
        scores[currPlayerIdx] = 0;
    }
    else {
        betChips[currPlayerIdx] += diffBet;
        scores[currPlayerIdx] -= diffBet;
    }
}

// TODO: Remove player with 0 scores after split pots
void HoldEmGame::calculateScore() {
    std::cout << "----------Calculating scores!----------" << std::endl;
    if (state == HoldEmState::flop) {
        for (size_t i = 0; i < players.size(); i++) {
            if (!foldState[i]) {
                scores[i] += commonPot;
            }
        }
    }
    else {
        std::vector<HoldEmGameStruct> tempStructs;
        for (size_t i = 0; i < players.size(); i++) {
            if (!foldState[i]) {
                tempStructs.push_back(structs[i]);
            }
        }
        std::sort(tempStructs.begin(), tempStructs.end(), operator<);
        // Single best
        if (tempStructs[players.size() -1].rank != tempStructs[players.size() -2].rank) {
            size_t bestPlayeridx = tempStructs.back().playerIdx;
            scores[bestPlayeridx] += commonPot;
        } // multiple best
        else {
            // get the number of multiple best players
            int i;
            for (i = tempStructs.size() - 1; i > 0; i--) {
                if (operaterHelper(tempStructs[i-1], tempStructs[i]) != equalRank) {
                    break;
                }
            }
            int numMultipleBest = tempStructs.size() - i;
            int splitPot = commonPot / numMultipleBest;
            int remainPot = commonPot % numMultipleBest;
            // split and get their idx
            std::vector<size_t> multiplePlayerIdx;
            for (size_t j = i; j < tempStructs.size(); j++) {
                scores[tempStructs[j].playerIdx] += splitPot;
                multiplePlayerIdx.push_back(tempStructs[j].playerIdx);
            }
            // find the earlier player idx and add remain pot to the player
            for (size_t i = 1; i < players.size() + 1; i++) {
                size_t nextIdx = (dealer + i) % players.size();
                auto it = std::find(multiplePlayerIdx.begin(), multiplePlayerIdx.end(), nextIdx);
                if (it != multiplePlayerIdx.end()) {
                    int idx = it - multiplePlayerIdx.begin();
                    size_t earlierPlayerIdx = multiplePlayerIdx[idx];
                    scores[earlierPlayerIdx] += remainPot;
                    break;
                }
            } 
        }
    }
    for (size_t i = 0; i < players.size(); i++) {
        std::cout << players[i] << " score: " << scores[i] << std::endl;
    }
    std::cout << "" << std::endl;
    commonPot = 0;
}

int HoldEmGame::evaluateEndRoundOrGame() {
    // Only one unfold -> end game
    if (std::count(foldState.begin(), foldState.end(), false) == 1) {
        return endGame;
    }
    // All unfold hve 0 scores(chips) left -> end game
    bool scoresEvaluate = true;
    for (size_t i = 0; i < players.size(); i++) {
        if (!foldState[i]) {
            if (scores[i] != 0) {
                scoresEvaluate = false;
                break;
            }
        }
    }
    if (scoresEvaluate) {
        return endGame;
    }
    // All unfold are called -> end this round
    bool calledEvaluate = true;
    for (size_t i = 0; i < players.size(); i++) {
        if (!foldState[i] && !callState[i]) {
            calledEvaluate = false;
            break;
        }
    }
    if (calledEvaluate) {
        return endRound;
    }
    return continueBet;
}

void HoldEmGame::actRaiseOrCall(const HoldEmRaiseCallState &raiseCallState, const int &currPlayerIdx) {
    int largestBet = *std::max_element(betChips.begin(), betChips.end());
    if ((raiseCallState == HoldEmRaiseCallState::raiseAlways) || 
        ((raiseCallState == HoldEmRaiseCallState::raiseOnce) && raiseTimes[currPlayerIdx] == 0)) {
        int raiseAim;
        if ((state == HoldEmState::flop) || (state == HoldEmState::turn)) {
            raiseAim = largestBet + smallRaise;
        }
        else {
            raiseAim = largestBet + bigRaise;
        }
        unsigned int diffBet = raiseAim - betChips[currPlayerIdx];
        processChips(currPlayerIdx, diffBet);
        raiseTimes[currPlayerIdx] += 1;
    }
    else if ((raiseCallState == HoldEmRaiseCallState::raiseOnce) || 
        (raiseCallState == HoldEmRaiseCallState::callAlways)) {
        unsigned int diffBet = largestBet - betChips[currPlayerIdx];
        processChips(currPlayerIdx, diffBet);
        callState[currPlayerIdx] = true;
    }
}

// FIXME: player with 0 scores(unsigned int) will overflow (small/big blind), 
// can be fixed after finish calculate score func (remove 0 score players)
bool HoldEmGame::preflopBet() {
    commonPot = 0;
    foldState.assign(players.size(), false);
    callState = foldState;
    int smallBlindIdx = (dealer + 1) % players.size();
    int bigBlindIdx = (smallBlindIdx + 1) % players.size();
    betChips[smallBlindIdx] = smallBlindBet;
    betChips[bigBlindIdx] = bigBlindBet;
    scores[smallBlindIdx] -= smallBlindBet;
    scores[bigBlindIdx] -= bigBlindBet;
    int currPlayerIdx = (bigBlindIdx + 1) % players.size();

    while (true)
    {   
        if (foldState[currPlayerIdx] || scores[currPlayerIdx] == 0) {
            callState[currPlayerIdx] = true;
            currPlayerIdx = (currPlayerIdx + 1) % players.size();
            continue;
        }

        HoldEmRaiseCallState preflopState = evaluatePreflopState(hands[currPlayerIdx]);
        actRaiseOrCall(preflopState, currPlayerIdx);
        if (preflopState == HoldEmRaiseCallState::foldOrCheck) {
            foldState[currPlayerIdx] = true;
            callState[currPlayerIdx] = true;
        }

        int evaluation = evaluateEndRoundOrGame();
        if (evaluation == endGame) {
            return true;
        }
        else if (evaluation == endRound) {
            return false;
        }
        currPlayerIdx = (currPlayerIdx + 1) % players.size();
    }
    return false;
}

bool HoldEmGame::postflopBet() {
    callState = foldState;
    int currPlayerIdx = (dealer + 1) % players.size();

    while (true) {
        if (foldState[currPlayerIdx] || scores[currPlayerIdx] == 0) {
            callState[currPlayerIdx] = true;
            currPlayerIdx = (currPlayerIdx + 1) % players.size();
            continue;
        }

        HoldEmRaiseCallState postflopState;
        switch (state)
        {
        case HoldEmState::turn:
            postflopState = evaluatePostFlopState(structs[currPlayerIdx]);
            break;
        case HoldEmState::river:
            postflopState = evaluatePostTurnState(structs[currPlayerIdx]);
            break;
        case HoldEmState::undefined:
            postflopState = evaluatePostRiverState(structs[currPlayerIdx]);
            break;
        default:
            postflopState = HoldEmRaiseCallState::undefined;
            break;
        }

        actRaiseOrCall(postflopState, currPlayerIdx);
        if (postflopState == HoldEmRaiseCallState::foldOrCheck) {
            if (std::reduce(raiseTimes.begin(), raiseTimes.end()) > 0) {
                foldState[currPlayerIdx] = true;
            }
            callState[currPlayerIdx] = true;
        }

        int evaluation = evaluateEndRoundOrGame();
        if (evaluation == endGame) {
            return true;
        }
        else if (evaluation == endRound) {
            return false;
        }
        currPlayerIdx = (currPlayerIdx + 1) % players.size();
    }
    return false;
}

HoldEmRaiseCallState HoldEmGame::evaluatePreflopState(const cardSetType &hand) {
    std::vector<cardType> cardSetType::*cards = cardSetType::getCards();
    if ((hand.*cards)[0].rank == (hand.*cards)[1].rank) {
        if ((hand.*cards)[0].rank == HoldEmRank::ace) {
            return HoldEmRaiseCallState::raiseAlways;
        }
        else if ((hand.*cards)[0].rank == HoldEmRank::king || 
            (hand.*cards)[0].rank == HoldEmRank::queen ||
            (hand.*cards)[0].rank == HoldEmRank::jack) {
            return HoldEmRaiseCallState::raiseOnce;
        }
        else {
            return HoldEmRaiseCallState::callAlways;
        }
    }
    if ((hand.*cards)[0].suit == (hand.*cards)[1].suit) {
        return HoldEmRaiseCallState::callAlways;
    }
    int rank1 = static_cast<int>((hand.*cards)[0].rank);
    int rank2 = static_cast<int>((hand.*cards)[1].rank);
    int minRank = static_cast<int>(HoldEmRank::three);
    if (((rank1 - rank2) == 1 && rank2 > minRank) || ((rank2 - rank1) == 1 && rank1 > minRank)) {
        return HoldEmRaiseCallState::callAlways;
    }
    return HoldEmRaiseCallState::foldOrCheck;
}

HoldEmRaiseCallState HoldEmGame::evaluatePostFlopState(const HoldEmGameStruct &currStruct) {
    HoldEmHandRank handRank = currStruct.rank;
    if (static_cast<int>(handRank) >= static_cast<int>(HoldEmHandRank::threeofakind)) {
        return HoldEmRaiseCallState::raiseAlways;
    }
    else if (handRank == HoldEmHandRank::twopair) {
        return HoldEmRaiseCallState::raiseOnce;
    }
    else if (handRank == HoldEmHandRank::pair) {
        return HoldEmRaiseCallState::callAlways;
    }
    return evaluateFourCards(currStruct);
}

HoldEmRaiseCallState HoldEmGame::evaluatePostTurnState(const HoldEmGameStruct &currStruct) {
    HoldEmHandRank handRank = currStruct.rank;
    if (static_cast<int>(handRank) >= static_cast<int>(HoldEmHandRank::straight)) {
        return HoldEmRaiseCallState::raiseAlways;
    }
    else if (handRank == HoldEmHandRank::threeofakind) {
        return HoldEmRaiseCallState::raiseOnce;
    }
    else if (handRank == HoldEmHandRank::pair || handRank == HoldEmHandRank::twopair) {
        return HoldEmRaiseCallState::callAlways;
    }
    return evaluateFourCards(currStruct);
}

HoldEmRaiseCallState HoldEmGame::evaluatePostRiverState(const HoldEmGameStruct &currStruct) {
    HoldEmHandRank handRank = currStruct.rank;
    if (static_cast<int>(handRank) >= static_cast<int>(HoldEmHandRank::straight)) {
        return HoldEmRaiseCallState::raiseAlways;
    }
    else if (handRank == HoldEmHandRank::threeofakind) {
        return HoldEmRaiseCallState::raiseOnce;
    }
    else if (handRank == HoldEmHandRank::twopair) {
        return HoldEmRaiseCallState::callAlways;
    }
    return HoldEmRaiseCallState::foldOrCheck;
}

HoldEmRaiseCallState HoldEmGame::evaluateFourCards(const HoldEmGameStruct &currStruct) {
    for (size_t i = 0; i < numFinalHands; i++) {
        std::vector<cardType> cardSetType::*cards = cardSetType::getCards();
        cardSetType handCopy = currStruct.cardSet;
        // remove 1 card from all 5 cards
        (handCopy.*cards).erase((handCopy.*cards).begin() + i);
        if (isFlush(handCopy.*cards)) {
            return HoldEmRaiseCallState::callAlways;
        }
        std::sort((handCopy.*cards).begin(), (handCopy.*cards).end(),
            compareByRank<HoldEmRank, Suit>);
        if (isStraight(handCopy.*cards) && (handCopy.*cards).back().rank != HoldEmRank::ace) {
            return HoldEmRaiseCallState::callAlways;
        }
    }
    return HoldEmRaiseCallState::foldOrCheck;
}

void HoldEmGame::collectHands() {
    for (cardSetType &hand : hands) {
        deck.collect(hand);
    }
}

void HoldEmGame::collectBoards() { deck.collect(commonBoards); }

bool HoldEmGame::endingGame() {
    calculateScore();
    collectHands();
    collectBoards();
    dealer = (dealer + 1) % players.size();
    return continuePrompt();
}

int HoldEmGame::play() {
    while (true) {
        deck.shuffle();
        state = HoldEmState::preflop;
        deal(); // state: flop
        printHands();
        if (bet()) {
            if (endingGame()) {
                return SUCCESS;
            }
            else {
                continue;
            }
        }

        deal(); // state: turn
        std::cout << "BOARD(flop): " << std::endl;
        printBoards();
        printSortedHands();
        if (bet()) {
            if (endingGame()) {
                return SUCCESS;
            }
            else {
                continue;
            }
        }

        deal(); // state: river
        std::cout << "BOARD(turn): " << std::endl;
        printBoards();
        printSortedHands();
        if (bet()) {
            if (endingGame()) {
                return SUCCESS;
            }
            else {
                continue;
            }
        }

        deal(); // state: undefined
        std::cout << "BOARD(river): " << std::endl;
        printBoards();
        printSortedHands();
        bet();
        if (endingGame()) {
            return SUCCESS;
        }
        else {
            continue;
        }
    }
    return SUCCESS;
}

HoldEmHandRank HoldEmGame::holdem_hand_evaluation(const cardSetType &hand) {
    cardSetType handCopy = hand;
    std::vector<cardType> cardSetType::*cards = cardSetType::getCards();

    // undefined
    if ((handCopy.*cards).size() != numFinalHands) {
        return HoldEmHandRank::undefined;
    }

    std::sort((handCopy.*cards).begin(), (handCopy.*cards).end(),
              compareByRank<HoldEmRank, Suit>);

    bool flush = isFlush(handCopy.*cards);
    bool straight = isStraight(handCopy.*cards);

    // straight flush
    if (flush && straight) {
        return HoldEmHandRank::straightflush;
    }

    // four of a kind
    if (isFourOfAKind(handCopy.*cards)) {
        return HoldEmHandRank::fourofakind;
    }

    // full house
    if (isFullHouse(handCopy.*cards)) {
        return HoldEmHandRank::fullhouse;
    }

    // flush
    if (flush) {
        return HoldEmHandRank::flush;
    }

    // straight
    if (straight) {
        return HoldEmHandRank::straight;
    }

    // three of a kind
    if (isThreeOfAKind(handCopy.*cards)) {
        return HoldEmHandRank::threeofakind;
    }

    // two pair
    if (isTwoPair(handCopy.*cards)) {
        return HoldEmHandRank::twopair;
    }

    // pair
    if (isPair(handCopy.*cards)) {
        return HoldEmHandRank::pair;
    }

    // xhigh
    return HoldEmHandRank::xhigh;
}

bool HoldEmGame::isFlush(const std::vector<cardType> &cards) {
    for (size_t i = 1; i < cards.size(); i++) {
        if (cards[i].suit != cards[i - 1].suit) {
            return false;
        }
    }
    return true;
}

bool HoldEmGame::isStraight(const std::vector<cardType> &cards) {
    for (size_t i = 1; i < cards.size(); i++) {
        int prevRank = static_cast<int>(cards[i - 1].rank);
        int currRank = static_cast<int>(cards[i].rank);
        if (prevRank + 1 != currRank &&
            !(cards[i - 1].rank == HoldEmRank::five &&
              cards[i].rank == HoldEmRank::ace)) {
            return false;
        }
    }
    return true;
}

bool HoldEmGame::isFourOfAKind(const std::vector<cardType> &cards) {
    return cards[0].rank == cards[3].rank || cards[1].rank == cards[4].rank;
}

bool HoldEmGame::isFullHouse(const std::vector<cardType> &cards) {
    return (cards[0].rank == cards[1].rank && cards[2].rank == cards[4].rank) ||
           (cards[0].rank == cards[2].rank && cards[3].rank == cards[4].rank);
}

bool HoldEmGame::isThreeOfAKind(const std::vector<cardType> &cards) {
    return cards[0].rank == cards[2].rank || cards[1].rank == cards[3].rank ||
           cards[2].rank == cards[4].rank;
}

bool HoldEmGame::isTwoPair(const std::vector<cardType> &cards) {
    return (cards[0].rank == cards[1].rank && cards[2].rank == cards[3].rank) ||
           (cards[0].rank == cards[1].rank && cards[3].rank == cards[4].rank) ||
           (cards[1].rank == cards[2].rank && cards[3].rank == cards[4].rank);
}

bool HoldEmGame::isPair(const std::vector<cardType> &cards) {
    return cards[0].rank == cards[1].rank || cards[1].rank == cards[2].rank ||
           cards[2].rank == cards[3].rank || cards[3].rank == cards[4].rank;
}

std::ostream &operator<<(std::ostream &os, const HoldEmHandRank &rank) {
    switch (rank) {
        case HoldEmHandRank::xhigh:
            os << "X-High";
            break;
        case HoldEmHandRank::pair:
            os << "Pair";
            break;
        case HoldEmHandRank::twopair:
            os << "Two Pair";
            break;
        case HoldEmHandRank::threeofakind:
            os << "Three of a Kind";
            break;
        case HoldEmHandRank::straight:
            os << "Straight";
            break;
        case HoldEmHandRank::flush:
            os << "Flush";
            break;
        case HoldEmHandRank::fullhouse:
            os << "Full House";
            break;
        case HoldEmHandRank::fourofakind:
            os << "Four of a Kind";
            break;
        case HoldEmHandRank::straightflush:
            os << "Straight Flush";
            break;
        default:
            os << "Undefined";
            break;
    }
    return os;
}

bool operator<(const HoldEmGame::HoldEmGameStruct &obj1,
               const HoldEmGame::HoldEmGameStruct &obj2) {
    return operaterHelper(obj1, obj2) == HoldEmGame::lowerThan;
}

int operaterHelper(const HoldEmGame::HoldEmGameStruct &obj1, 
        const HoldEmGame::HoldEmGameStruct &obj2) {
    using cardType = Card<HoldEmRank, Suit>;
    using cardSetType = CardSet<HoldEmRank, Suit>;

    if (obj1.rank < obj2.rank) {
        return HoldEmGame::lowerThan;
    } else if (obj1.rank > obj2.rank) {
        return HoldEmGame::higherThan;
    }

    cardSetType hand1 = obj1.cardSet;
    cardSetType hand2 = obj2.cardSet;
    std::vector<cardType> cardSetType::*cards = cardSetType::getCards();

    // Sort by rank from A-2 high to low
    std::sort((hand1.*cards).begin(), (hand1.*cards).end(),
              compareByRank<HoldEmRank, Suit>);
    std::reverse((hand1.*cards).begin(), (hand1.*cards).end());
    std::sort((hand2.*cards).begin(), (hand2.*cards).end(),
              compareByRank<HoldEmRank, Suit>);
    std::reverse((hand2.*cards).begin(), (hand2.*cards).end());

    // Both pair
    if (obj1.rank == HoldEmHandRank::pair) {
        return HoldEmGame::sortPair(hand1, hand2);
    }

    // Both twopair
    if (obj1.rank == HoldEmHandRank::twopair) {
        return HoldEmGame::sortTwoPair(hand1, hand2);
    }

    // Both threeofakind or fullhouse
    if (obj1.rank == HoldEmHandRank::threeofakind ||
        obj1.rank == HoldEmHandRank::fullhouse) {
        return HoldEmGame::sortAnyThree(hand1, hand2);
    }

    // Both straight or straightflush
    if (obj1.rank == HoldEmHandRank::straight ||
        obj1.rank == HoldEmHandRank::straightflush) {
        return HoldEmGame::sortAnyStraight(hand1, hand2);
    }

    // Both flush or xhigh
    if (obj1.rank == HoldEmHandRank::flush ||
        obj1.rank == HoldEmHandRank::xhigh) {
        return HoldEmGame::sortFlushXhigh(hand1, hand2);
    }

    // Both fourofakind
    if (obj1.rank == HoldEmHandRank::fourofakind) {
        return HoldEmGame::sortFourOfAKind(hand1, hand2);
    }

    return HoldEmGame::compareFail;
}

int HoldEmGame::sortPair(const cardSetType &hand1, const cardSetType &hand2) {
    std::vector<cardType> cardSetType::*cards = cardSetType::getCards();
    cardSetType handCopy1 = hand1;
    cardSetType handCopy2 = hand2;
    // get the pair rank
    HoldEmRank pairRank1, pairRank2;
    for (size_t i = 1; i < (hand1.*cards).size(); i++) {
        if ((hand1.*cards)[i - 1].rank == (hand1.*cards)[i].rank) {
            pairRank1 = (hand1.*cards)[i].rank;
            // remove pair cards
            (handCopy1.*cards)
                .erase((handCopy1.*cards).begin() + (i - 1),
                       (handCopy1.*cards).begin() + (i + 1));
        }
        if ((hand2.*cards)[i - 1].rank == (hand2.*cards)[i].rank) {
            pairRank2 = (hand2.*cards)[i].rank;
            // remove pair cards
            (handCopy2.*cards)
                .erase((handCopy2.*cards).begin() + (i - 1),
                       (handCopy2.*cards).begin() + (i + 1));
        }
    }
    // compare pair rank
    if (pairRank1 < pairRank2) {
        return HoldEmGame::lowerThan;
    } else if (pairRank1 > pairRank2) {
        return HoldEmGame::higherThan;
    }
    // compare other cards rank from high to low
    else {
        for (size_t i = 0; i < (handCopy1.*cards).size(); i++) {
            if ((handCopy1.*cards)[i].rank < (handCopy2.*cards)[i].rank) {
                return HoldEmGame::lowerThan;
            } else if ((handCopy1.*cards)[i].rank > (handCopy2.*cards)[i].rank) {
                return HoldEmGame::higherThan;
            }
        }
    }
    return HoldEmGame::equalRank;
}

int HoldEmGame::sortTwoPair(const cardSetType &hand1,
                             const cardSetType &hand2) {
    std::vector<cardType> cardSetType::*cards = cardSetType::getCards();
    // get the two pairs rank
    std::vector<HoldEmRank> pairRank1, pairRank2;
    for (size_t i = 1; i < (hand1.*cards).size(); i++) {
        if ((hand1.*cards)[i - 1].rank == (hand1.*cards)[i].rank) {
            pairRank1.push_back((hand1.*cards)[i].rank);
        }
        if ((hand2.*cards)[i - 1].rank == (hand2.*cards)[i].rank) {
            pairRank2.push_back((hand2.*cards)[i].rank);
        }
    }
    // compare pair ranks from high to low
    for (size_t i = 0; i < pairRank1.size(); i++) {
        if (pairRank1[i] < pairRank2[i]) {
            return HoldEmGame::lowerThan;
        } else if (pairRank1[i] > pairRank2[i]) {
            return HoldEmGame::higherThan;
        }
    }
    // get the other card
    HoldEmRank nonPairRank1, nonPairRank2;
    for (size_t i = 0; i < (hand1.*cards).size(); i++) {
        if ((hand1.*cards)[i].rank != pairRank1[0] &&
            (hand1.*cards)[i].rank != pairRank1[1]) {
            nonPairRank1 = (hand1.*cards)[i].rank;
        }
        if ((hand2.*cards)[i].rank != pairRank2[0] &&
            (hand2.*cards)[i].rank != pairRank2[1]) {
            nonPairRank2 = (hand2.*cards)[i].rank;
        }
    }
    // compare the other card rank
    if (nonPairRank1 < nonPairRank2) {
        return HoldEmGame::lowerThan;
    }
    else if (nonPairRank1 > nonPairRank2) {
        return HoldEmGame::higherThan;
    }
    return HoldEmGame::equalRank;
}

int HoldEmGame::sortAnyThree(const cardSetType &hand1,
                              const cardSetType &hand2) {
    std::vector<cardType> cardSetType::*cards = cardSetType::getCards();
    // get the three of a kind rank
    HoldEmRank pairRank1, pairRank2;
    for (size_t i = 2; i < (hand1.*cards).size(); i++) {
        if ((hand1.*cards)[i - 1].rank == (hand1.*cards)[i].rank &&
            (hand1.*cards)[i - 2].rank == (hand1.*cards)[i].rank) {
            pairRank1 = (hand1.*cards)[i].rank;
        }
        if ((hand2.*cards)[i - 1].rank == (hand2.*cards)[i].rank &&
            (hand2.*cards)[i - 2].rank == (hand2.*cards)[i].rank) {
            pairRank2 = (hand2.*cards)[i].rank;
        }
    }
    // compare pair rank
    if (pairRank1 < pairRank2) {
        return HoldEmGame::lowerThan;
    }
    else if (pairRank1 > pairRank2) {
        return HoldEmGame::higherThan;
    }
    return HoldEmGame::equalRank;
}

int HoldEmGame::sortAnyStraight(const cardSetType &hand1,
                                 const cardSetType &hand2) {
    std::vector<cardType> cardSetType::*cards = cardSetType::getCards();
    if ((hand1.*cards)[0].rank < (hand2.*cards)[0].rank) {
        return HoldEmGame::lowerThan;
    }
    else if ((hand1.*cards)[0].rank > (hand2.*cards)[0].rank) {
        return HoldEmGame::higherThan;
    }
    return HoldEmGame::equalRank;
}

int HoldEmGame::sortFlushXhigh(const cardSetType &hand1,
                                const cardSetType &hand2) {
    std::vector<cardType> cardSetType::*cards = cardSetType::getCards();
    for (size_t i = 0; i < (hand1.*cards).size(); i++) {
        if ((hand1.*cards)[i].rank < (hand2.*cards)[i].rank) {
            return HoldEmGame::lowerThan;
        } else if ((hand1.*cards)[i].rank > (hand2.*cards)[i].rank) {
            return HoldEmGame::higherThan;
        }
    }
    return HoldEmGame::equalRank;
}
int HoldEmGame::sortFourOfAKind(const cardSetType &hand1,
                                 const cardSetType &hand2) {
    std::vector<cardType> cardSetType::*cards = cardSetType::getCards();
    // get pair
    HoldEmRank pairRank1 = (hand1.*cards)[1].rank;
    HoldEmRank pairRank2 = (hand2.*cards)[1].rank;
    // compare pair rank
    if (pairRank1 < pairRank2) {
        return HoldEmGame::lowerThan;
    }
    else if (pairRank1 > pairRank2) {
        return HoldEmGame::higherThan;
    }
    return HoldEmGame::equalRank;
}