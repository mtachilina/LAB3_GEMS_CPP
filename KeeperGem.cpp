#include "KeeperGem.hpp"

void KeeperGem::draw(sf::RenderWindow& window, const sf::Color colors[], int cellSize, float offsetX, float offsetY) {

    sf::RectangleShape shape(sf::Vector2f(cellSize - 2.f, cellSize - 2.f));
    shape.setPosition(offsetX, offsetY);
    shape.setFillColor(colors[color]);
    window.draw(shape);
}
