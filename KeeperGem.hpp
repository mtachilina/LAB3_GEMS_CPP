#pragma once
#include "Gem.hpp"

class KeeperGem : public Gem {
public:
    using Gem::Gem;

    void draw(sf::RenderWindow& window, const sf::Color colors[], int cellSize, float offsetX, float offsetY);
};
