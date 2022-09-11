#pragma once
#include <iostream>
using namespace std;

enum class Suit { clubs, diamonds, hearts, spades, undefined };

ostream& operator<<(ostream& os, const Suit& suit);

Suit& operator++(Suit& suit);