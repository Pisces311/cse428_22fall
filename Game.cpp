#include "Game.h"

#include <iostream>
#include <string>
#include <vector>

Game::Game(int argc, const char **argv) {
    for (int i = startIndex; i < argc; ++i) {
        players.push_back(argv[i]);
    }
    dealer = players.size() - 1;
    scores.assign(players.size(), initialScore);
}

bool Game::continuePrompt() {
    std::cout << "End the game? (yes/no): ";
    std::string answer;
    std::cin >> answer;
    return answer == "yes";
}