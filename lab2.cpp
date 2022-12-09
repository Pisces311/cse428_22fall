#include <memory>

#include "HoldEmGame.h"
#include "PinochleGame.h"

using std::make_shared;
using std::shared_ptr;

const int COMMAND_LINE_ARG_ERROR = 1;
const int CREATE_GAME_OBJECT_ERROR = 2;
const int GAME_NAME_INDEX = 1;
const int PINOCHLE_PLAYER_NUM = 4;
const int HOLDEM_PLAYER_NUM_MIN = 2;
const int HOLDEM_PLAYER_NUM_MAX = 9;

shared_ptr<Game> create(int argc, const char **argv) {
    shared_ptr<Game> spGame;
    std::string gameName = argv[GAME_NAME_INDEX];

    if (gameName == "Pinochle") {
        spGame = make_shared<PinochleGame>(argc, argv);
    } else if (gameName == "HoldEm") {
        spGame = make_shared<HoldEmGame>(argc, argv);
    }

    return spGame;
}

void printUsage(const char *name) {
    std::cout << "Usage: " << name << " Pinochle/HoldEm player1 player2 ..."
              << std::endl;
    std::cout << "[Pinochle]: must have 4 players" << std::endl;
    std::cout << "[HoldEm]: between 2 and 9 players" << std::endl;
}

int main(int argc, const char **argv) {
    if (argc == 1) {
        printUsage(argv[0]);
        return COMMAND_LINE_ARG_ERROR;
    }

    // Check command line arguments
    std::string gameName = argv[GAME_NAME_INDEX];
    if ((gameName == "Pinochle" && argc == 2 + PINOCHLE_PLAYER_NUM) ||
        (gameName == "HoldEm" && argc >= 2 + HOLDEM_PLAYER_NUM_MIN &&
         argc <= 2 + HOLDEM_PLAYER_NUM_MAX)) {
        shared_ptr<Game> spGame = create(argc, argv);
        if (spGame) {
            return spGame->play();
        } else {
            std::cout << "Creating game object failed." << std::endl;
            return CREATE_GAME_OBJECT_ERROR;
        }
    } else {
        printUsage(argv[0]);
        return COMMAND_LINE_ARG_ERROR;
    }
}