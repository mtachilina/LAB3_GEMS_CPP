#pragma once
#include <SFML/Graphics.hpp>

class Board;

class Gem {
protected:
    int color;
    int x, y;

public:
    Gem(int c = 0, int px = 0, int py = 0);
    virtual ~Gem() = default;

    virtual void draw(sf::RenderWindow& window, const sf::Color colors[], int cellSize, float offsetX, float offsetY) = 0;

    virtual bool isBonus() const { return false; }
    virtual void activate(class Board& board, int i, int j);

    int getColor() const;
    void setColor(int c);

    void setPosition(int px, int py);
    int getX() const;
    int getY() const;
};
