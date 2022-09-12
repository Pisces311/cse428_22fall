#pragma once

#include <iostream>

class Deck {
   public:
    virtual void print(std::ostream& os) = 0;
    virtual ~Deck() = default;
};