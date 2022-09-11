#pragma once

#include <iostream>
using namespace std;

class Deck {
    public:
        virtual void print(ostream& os) = 0;
};