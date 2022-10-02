#pragma once

#include <string>
#include <vector>

class Game {
   protected:
    const int SUCCESS = 0;

    int startIndex = 2;
    std::vector<std::string> players;

    bool continuePrompt();

   public:
    Game(int argc, const char **argv);
    virtual ~Game() = default;
    virtual int play() = 0;
};
