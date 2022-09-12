#pragma once
#include <iostream>

enum class Suit { clubs, diamonds, hearts, spades, undefined };

std::ostream& operator<<(std::ostream& os, const Suit& suit);

Suit& operator++(Suit& suit);