#pragma once
#include "Gem.hpp"

class BombGem : public Gem {
public:
    using Gem::Gem;

    bool isBonus() const override { return true; }
    void activate(Board& board, int i, int j) override;

    void draw(sf::RenderWindow& window,
        const sf::Color colors[],
        int cellSize,
        float offsetX,
        float offsetY) override;
};
