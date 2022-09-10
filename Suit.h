#pragma once

enum class Suit { clubs, diamonds, hearts, spades, undefined };

std::ostream& operator<<(std::ostream& os, Suit suit);

Suit& operator++(Suit& suit);