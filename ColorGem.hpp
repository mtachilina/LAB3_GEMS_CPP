#pragma once
#include "Gem.hpp"

class ColorGem : public Gem {
public:
    using Gem::Gem;

    bool isBonus() const override { return true; }

    void draw(sf::RenderWindow& window,
        const sf::Color colors[],
        int cellSize,
        float offsetX,
        float offsetY) override;
};