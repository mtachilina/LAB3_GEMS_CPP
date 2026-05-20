#pragma once
#include <SFML/Graphics.hpp>

class Board {
private:
    static const int SIZE = 8;
    static const int CELL_SIZE = 50;

    int grid[SIZE][SIZE];
    sf::RectangleShape cells[SIZE][SIZE];

    sf::Color colors[5];

    bool firstSelected = false;
    int selX = -1, selY = -1;

    bool findMatches();
    void applyGravity();

    void spawnBonus(int i, int j);
    void applyColorBonus(int i, int j, int color);
    void applyBombBonus(int i, int j);

public:
    Board();
    void draw(sf::RenderWindow& window);
    void handleClick(int x, int y);
};
