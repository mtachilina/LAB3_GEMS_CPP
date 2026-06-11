#include <SFML/Graphics.hpp>
#include "Board.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode(GameConfig::WINDOW_SIZE, GameConfig::WINDOW_SIZE), "GEMS");

    Board board;

    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                board.handleClick(event.mouseButton.x, event.mouseButton.y);
            }
        }

        window.clear(sf::Color::Black);
        board.draw(window);
        window.display();
    }
    return 0;
}
