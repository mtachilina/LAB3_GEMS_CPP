#include "Gem.hpp"

Gem::Gem(int c, int px, int py) : color(c), x(px), y(py) {}

void Gem::activate(Board& board, int i, int j) {}

int Gem::getColor() const { return color; }
void Gem::setColor(int c) { color = c; }

void Gem::setPosition(int px, int py) { x = px; y = py; }

int Gem::getX() const { return x; }
int Gem::getY() const { return y; }
