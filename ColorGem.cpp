#include "ColorGem.hpp"
#include "Board.hpp"

void ColorGem::activate(Board& board, int i, int j) {
    int savedColor = getColor();
    board.activateColor(i, j, savedColor);
}

void ColorGem::draw(sf::RenderWindow& window, const sf::Color colors[], int cellSize, float offsetX, float offsetY) {

    sf::RectangleShape shape(sf::Vector2f(cellSize - 2.f, cellSize - 2.f));
    shape.setPosition(offsetX, offsetY);
    shape.setFillColor(colors[color]);
    window.draw(shape);

    sf::CircleShape dot(cellSize / 4.f);
    dot.setPosition(offsetX + cellSize / 4.f, offsetY + cellSize / 4.f);
    dot.setFillColor(sf::Color::White);
    window.draw(dot);
}
