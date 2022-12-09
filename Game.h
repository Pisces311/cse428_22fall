#pragma once

#include <string>
#include <vector>

class Game {
   protected:
    const int SUCCESS = 0;

    size_t dealer = 0;

    int startIndex = 2;
    int initialScore = 0;
    std::vector<std::string> players;
    std::vector<unsigned int> scores;

    bool continuePrompt();

   public:
    Game(int argc, const char **argv);
    virtual ~Game() = default;
    virtual int play() = 0;
};
