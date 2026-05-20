#include "Board.hpp"
#include <cstdlib>
#include <ctime>
#include <cmath>

Board::Board()
    : firstSelected(false), selX(-1), selY(-1)
{
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    colors[0] = sf::Color::Red;
    colors[1] = sf::Color::Green;
    colors[2] = sf::Color::Blue;
    colors[3] = sf::Color::Yellow;
    colors[4] = sf::Color::Magenta;

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            grid[i][j] = std::rand() % 5;

            cells[i][j].setSize(sf::Vector2f(CELL_SIZE - 2, CELL_SIZE - 2));
            cells[i][j].setPosition(j * CELL_SIZE, i * CELL_SIZE);
        }
    }
}

void Board::draw(sf::RenderWindow& window) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {

            if (grid[i][j] >= 0)
                cells[i][j].setFillColor(colors[grid[i][j]]);

            window.draw(cells[i][j]);
        }
    }
}

void Board::handleClick(int x, int y) {
    int i = y / CELL_SIZE;
    int j = x / CELL_SIZE;

    if (i < 0 || i >= SIZE || j < 0 || j >= SIZE)
        return;

    if (!firstSelected) {
        selX = j;
        selY = i;
        firstSelected = true;
        return;
    }

    bool isNeighbor = (std::abs(selX - j) + std::abs(selY - i) == 1);

    if (!isNeighbor) {
        firstSelected = false;
        return;
    }

    std::swap(grid[selY][selX], grid[i][j]);

    bool changed = true;

    while (changed) {
        changed = findMatches();

        if (changed)
            applyGravity();
    }

    firstSelected = false;
}

bool Board::findMatches() {
    bool del[SIZE][SIZE] = { false };
    bool found = false;

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE - 2; j++) {

            if (grid[i][j] != -1 &&
                grid[i][j] == grid[i][j + 1] &&
                grid[i][j] == grid[i][j + 2]) {

                del[i][j] = del[i][j + 1] = del[i][j + 2] = true;
                found = true;

                spawnBonus(i, j);
            }
        }
    }

    for (int j = 0; j < SIZE; j++) {
        for (int i = 0; i < SIZE - 2; i++) {

            if (grid[i][j] != -1 &&
                grid[i][j] == grid[i + 1][j] &&
                grid[i][j] == grid[i + 2][j]) {

                del[i][j] = del[i + 1][j] = del[i + 2][j] = true;
                found = true;

                spawnBonus(i, j);
            }
        }
    }

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (del[i][j])
                grid[i][j] = -1;
        }
    }

    return found;
}

void Board::applyGravity() {
    for (int j = 0; j < SIZE; j++) {

        for (int i = SIZE - 1; i >= 0; i--) {

            if (grid[i][j] != -1)
                continue;

            for (int k = i - 1; k >= 0; k--) {

                if (grid[k][j] == -1)
                    continue;

                grid[i][j] = grid[k][j];
                grid[k][j] = -1;
                break;
            }
        }
    }

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {

            if (grid[i][j] == -1)
                grid[i][j] = std::rand() % 5;
        }
    }
}

void Board::spawnBonus(int i, int j) {
    int r = std::rand() % 10;

    if (r < 3) {
        applyBombBonus(i, j);
        return;
    }

    if (r < 7) {
        applyColorBonus(i, j, grid[i][j]);
    }
}

void Board::applyBombBonus(int i, int j) {
    int removed = 0;

    for (int x = 0; x < SIZE && removed < 5; x++) {
        for (int y = 0; y < SIZE && removed < 5; y++) {

            if (std::abs(x - i) + std::abs(y - j) > 2)
                continue;

            if (grid[x][y] != -1) {
                grid[x][y] = -1;
                removed++;
            }
        }
    }
}

void Board::applyColorBonus(int i, int j, int color) {
    int changed = 0;

    for (int x = 0; x < SIZE && changed < 2; x++) {
        for (int y = 0; y < SIZE && changed < 2; y++) {

            if (std::abs(x - i) + std::abs(y - j) > 3)
                continue;

            if (x == i && y == j)
                continue;

            grid[x][y] = color;
            changed++;
        }
    }
}
