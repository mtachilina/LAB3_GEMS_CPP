#include "BombGem.hpp"

void BombGem::draw(sf::RenderWindow& window, const sf::Color colors[], int cellSize, float offsetX, float offsetY) {

    sf::RectangleShape shape(sf::Vector2f(cellSize - 2.f, cellSize - 2.f));
    shape.setPosition(offsetX, offsetY);
    shape.setFillColor(colors[color]);
    window.draw(shape);

    sf::CircleShape bomb(cellSize / 4.f);
    bomb.setPosition(offsetX + cellSize / 4.f, offsetY + cellSize / 4.f);
    bomb.setFillColor(sf::Color::Black);
    window.draw(bomb);
}
