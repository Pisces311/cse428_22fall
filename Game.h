#pragma once

#include <vector>
#include <string>

class Game {
    int startIndex = 2;

protected:
    std::vector<std::string> players;

public:
    Game(int argc, const char **argv);
    virtual ~Game() = default;
    virtual int play() = 0;
};
