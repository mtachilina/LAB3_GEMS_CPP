#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <random>
#include <set>
#include "Constants.hpp"
#include "Gem.hpp"
#include "GemFactory.hpp"

class Board {
private:
    std::unique_ptr<Gem> grid[GameConfig::GRID_SIZE][GameConfig::GRID_SIZE];
    sf::Color colors[GameConfig::COLORS_COUNT];

    bool firstSelected = false;
    int selX = -1, selY = -1;

    std::mt19937 rng;

    using Pos = std::pair<int, int>;

    bool isValid(int i, int j);
    int dist(int i1, int j1, int i2, int j2);
    bool canVisit(int i, int j, int color, bool visited[GameConfig::GRID_SIZE][GameConfig::GRID_SIZE]);

    void dfs(int i, int j, int color, bool visited[GameConfig::GRID_SIZE][GameConfig::GRID_SIZE], std::vector<Pos>& group);
    void processGroup(std::vector<Pos>& group, std::set<Pos>& result);
    std::set<Pos> findAllMatches();

    void separateCells(const std::set<Pos>& matches, std::vector<Pos>& bonusCells);
    void activateBonus(int i, int j);
    void applyMatches(const std::set<Pos>& matches);

    void applyGravity();
    void refill();
    void ensureNoInitialMatches();

    std::vector<Pos> collectSpots(int fromI, int fromJ);
    void trySpawnBonus(int fromI, int fromJ, int sourceColor);

    std::vector<Pos> collectAllCells(int exceptI, int exceptJ);
    void activateBomb(int i, int j);

    std::vector<Pos> collectCandidates(int i, int j);
    void repaintCell(int i, int j, int color);
    void activateColor(int i, int j, int sourceColor);

    void spawnBonusesFromMatches(const std::set<Pos>& matches);
    void collectFreshBonuses(const std::set<Pos>& matches, std::set<Pos>& freshBonuses);
    std::set<Pos> filterFreshBonuses(const std::set<Pos>& matches, const std::set<Pos>& freshBonuses);
    void processMatches(std::set<Pos>& matches, bool firstIteration);
    bool trySwap(int i, int j);

public:
    Board();
    void draw(sf::RenderWindow& window);
    void handleClick(int x, int y);
    void activateBomb(int i, int j);
    void activateColor(int i, int j, int sourceColor);
};
