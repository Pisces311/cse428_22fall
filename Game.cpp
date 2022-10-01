#include "Game.h"

#include <vector>
#include <string>

Game::Game(int argc, const char **argv) {
    for (int i = startIndex; i < argc; ++i) {
        players.push_back(argv[i]);
    }
}