#include "Board.hpp"
#include <algorithm>

Board::Board() : rng(std::random_device{}()) {

    colors[0] = sf::Color::Red;
    colors[1] = sf::Color::Green;
    colors[2] = sf::Color::Blue;
    colors[3] = sf::Color::Yellow;
    colors[4] = sf::Color::Magenta;

    std::uniform_int_distribution<int> dist(0, GameConfig::COLORS_COUNT - 1);

    for (int i = 0; i < GameConfig::GRID_SIZE; i++) {
        for (int j = 0; j < GameConfig::GRID_SIZE; j++) {
            grid[i][j] =
                GemFactory::createGem(dist(rng), j, i, GemType::Keeper);
        }
    }

    ensureNoInitialMatches();
}

bool Board::isValid(int i, int j) {
    return i >= 0 && i < GameConfig::GRID_SIZE && j >= 0 && j < GameConfig::GRID_SIZE;
}

int Board::dist(int i1, int j1, int i2, int j2) {
    return std::abs(i1 - i2) + std::abs(j1 - j2);
}

bool Board::canVisit(int i, int j, int color, bool visited[GameConfig::GRID_SIZE][GameConfig::GRID_SIZE]) {
    return isValid(i, j) && !visited[i][j] && grid[i][j] && grid[i][j]->getColor() == color;
}

void Board::dfs(int i, int j, int color,
    bool visited[GameConfig::GRID_SIZE][GameConfig::GRID_SIZE],
    std::vector<Pos>& group) {

    if (!canVisit(i, j, color, visited)) return;

    visited[i][j] = true;
    group.push_back({ i, j });

    dfs(i + 1, j, color, visited, group);
    dfs(i - 1, j, color, visited, group);
    dfs(i, j + 1, color, visited, group);
    dfs(i, j - 1, color, visited, group);
}

void Board::processGroup(std::vector<Pos>& group, std::set<Pos>& result) {
    if (group.size() >= GameConfig::MATCH_MIN) {
        for (auto& p : group) {
            result.insert(p);
        }
    }
}

std::set<Board::Pos> Board::findAllMatches() {
    bool visited[GameConfig::GRID_SIZE][GameConfig::GRID_SIZE] = { false };
    std::set<Pos> result;

    for (int i = 0; i < GameConfig::GRID_SIZE; i++) {
        for (int j = 0; j < GameConfig::GRID_SIZE; j++) {
            if (visited[i][j] || !grid[i][j]) continue;

            std::vector<Pos> group;
            int color = grid[i][j]->getColor();
            dfs(i, j, color, visited, group);
            processGroup(group, result);
        }
    }

    return result;
}

void Board::separateCells(const std::set<Pos>& matches,
    std::vector<Pos>& bonusCells) {
    for (auto [i, j] : matches) {
        if (!grid[i][j]) continue;

        if (grid[i][j]->isBonus()) {
            bonusCells.push_back({ i, j });
        }
        else {
            grid[i][j].reset();
        }
    }
}

void Board::activateBonus(int i, int j) {
    if (!grid[i][j]) return;
    grid[i][j]->activate(*this, i, j);
    grid[i][j].reset();
}

void Board::applyMatches(const std::set<Pos>& matches) {
    std::vector<Pos> bonusCells;
    separateCells(matches, bonusCells);

    for (auto [i, j] : bonusCells) {
        activateBonus(i, j);
    }
}

std::vector<Board::Pos> Board::collectSpots(int fromI, int fromJ) {
    std::vector<Pos> spots;
    for (int di = -GameConfig::BONUS_RADIUS; di <= GameConfig::BONUS_RADIUS; di++) {
        for (int dj = -GameConfig::BONUS_RADIUS; dj <= GameConfig::BONUS_RADIUS; dj++) {
            int ni = fromI + di;
            int nj = fromJ + dj;
            if (isValid(ni, nj) && dist(fromI, fromJ, ni, nj) <= GameConfig::BONUS_RADIUS) {
                spots.push_back({ ni, nj });
            }
        }
    }
    return spots;
}

void Board::trySpawnBonus(int fromI, int fromJ, int sourceColor) {
    std::uniform_int_distribution<int> chance(0, GameConfig::BONUS_MAX_ROLL - 1);
    if (chance(rng) >= GameConfig::BONUS_CHANCE) return;

    std::vector<Pos> spots = collectSpots(fromI, fromJ);
    if (spots.empty()) return;

    std::shuffle(spots.begin(), spots.end(), rng);
    auto [bi, bj] = spots[0];

    if (!grid[bi][bj] || grid[bi][bj]->isBonus()) return;

    std::uniform_int_distribution<int> type(0, 1);
    if (type(rng) == 0) {
        grid[bi][bj] = GemFactory::createGem(sourceColor, bj, bi, GemType::Bomb);
    }
    else {
        grid[bi][bj] = GemFactory::createGem(sourceColor, bj, bi, GemType::Color);
    }
}

std::vector<Board::Pos> Board::collectAllCells(int exceptI, int exceptJ) {
    std::vector<Pos> cells;
    cells.push_back({ exceptI, exceptJ });

    for (int x = 0; x < GameConfig::GRID_SIZE; x++) {
        for (int y = 0; y < GameConfig::GRID_SIZE; y++) {
            if ((x != exceptI || y != exceptJ) && grid[x][y]) {
                cells.push_back({ x, y });
            }
        }
    }
    return cells;
}

void Board::activateBomb(int i, int j) {
    std::vector<Pos> cells = collectAllCells(i, j);

    if (cells.size() > 1) {
        std::shuffle(cells.begin() + 1, cells.end(), rng);
    }

    int count = std::min(GameConfig::BOMB_TARGETS, (int)cells.size());
    for (int k = 0; k < count; k++) {
        auto [x, y] = cells[k];
        grid[x][y].reset();
    }
}

std::vector<Board::Pos> Board::collectCandidates(int i, int j) {
    std::vector<Pos> candidates;

    for (int di = -GameConfig::BONUS_RADIUS; di <= GameConfig::BONUS_RADIUS; di++) {
        for (int dj = -GameConfig::BONUS_RADIUS; dj <= GameConfig::BONUS_RADIUS; dj++) {
            int ni = i + di;
            int nj = j + dj;
            if (!isValid(ni, nj)) continue;
            if (ni == i && nj == j) continue;
            if (dist(i, j, ni, nj) > GameConfig::BONUS_RADIUS) continue;
            if (dist(i, j, ni, nj) == 1) continue;
            if (!grid[ni][nj]) continue;
            if (grid[ni][nj]->isBonus()) continue;
            candidates.push_back({ ni, nj });
        }
    }
    return candidates;
}

void Board::repaintCell(int i, int j, int color) {
    if (!isValid(i, j)) return;

    if (!grid[i][j]) {
        grid[i][j] = GemFactory::createGem(color, j, i, GemType::Keeper);
    }
    else if (!grid[i][j]->isBonus()) {
        grid[i][j]->setColor(color);
    }
}

void Board::activateColor(int i, int j, int sourceColor) {
    std::vector<Pos> candidates = collectCandidates(i, j);

    std::shuffle(candidates.begin(), candidates.end(), rng);

    repaintCell(i, j, sourceColor);

    int count = std::min(GameConfig::COLOR_REPAINTS, (int)candidates.size());
    for (int k = 0; k < count; k++) {
        auto [x, y] = candidates[k];
        grid[x][y]->setColor(sourceColor);
    }
}

void Board::applyGravity() {
    for (int j = 0; j < GameConfig::GRID_SIZE; j++) {
        int write = GameConfig::GRID_SIZE - 1;

        for (int i = GameConfig::GRID_SIZE - 1; i >= 0; i--) {
            if (!grid[i][j]) continue;
            grid[write][j] = std::move(grid[i][j]);
            grid[write][j]->setPosition(j, write);
            write--;
        }

        for (int i = write; i >= 0; i--) {
            grid[i][j] = nullptr;
        }
    }
}

void Board::refill() {
    std::uniform_int_distribution<int> dist(0, GameConfig::COLORS_COUNT - 1);

    for (int i = 0; i < GameConfig::GRID_SIZE; i++) {
        for (int j = 0; j < GameConfig::GRID_SIZE; j++) {
            if (grid[i][j]) continue;
            grid[i][j] =
                GemFactory::createGem(dist(rng), j, i, GemType::Keeper);
        }
    }
}

void Board::ensureNoInitialMatches() {
    auto matches = findAllMatches();

    while (!matches.empty()) {
        for (auto [i, j] : matches) {
            if (grid[i][j]) {
                grid[i][j].reset();
            }
        }
        applyGravity();
        refill();
        matches = findAllMatches();
    }
}

void Board::spawnBonusesFromMatches(const std::set<Pos>& matches) {
    for (auto [mi, mj] : matches) {
        if (!grid[mi][mj] || grid[mi][mj]->isBonus()) continue;
        int savedColor = grid[mi][mj]->getColor();
        trySpawnBonus(mi, mj, savedColor);
    }
}

void Board::collectFreshBonuses(const std::set<Pos>& matches,
    std::set<Pos>& freshBonuses) {
    for (auto [mi, mj] : matches) {
        if (grid[mi][mj] && grid[mi][mj]->isBonus()) {
            freshBonuses.insert({ mi, mj });
        }
    }
}

std::set<Board::Pos> Board::filterFreshBonuses(const std::set<Pos>& matches,
    const std::set<Pos>& freshBonuses) {
    std::set<Pos> filtered;
    for (auto& p : matches) {
        if (freshBonuses.find(p) == freshBonuses.end()) {
            filtered.insert(p);
        }
    }
    return filtered;
}

void Board::processMatches(std::set<Pos>& matches, bool firstIteration) {
    std::set<Pos> freshBonuses;

    if (firstIteration) {
        spawnBonusesFromMatches(matches);
    }

    collectFreshBonuses(matches, freshBonuses);
    applyMatches(matches);
    applyGravity();
    refill();
    matches = filterFreshBonuses(findAllMatches(), freshBonuses);
}

bool Board::trySwap(int i, int j) {
    std::swap(grid[selY][selX], grid[i][j]);
    grid[selY][selX]->setPosition(selX, selY);
    grid[i][j]->setPosition(j, i);

    auto matches = findAllMatches();

    if (matches.empty()) {
        std::swap(grid[selY][selX], grid[i][j]);
        grid[selY][selX]->setPosition(selX, selY);
        grid[i][j]->setPosition(j, i);
        return false;
    }

    bool firstIteration = true;
    do {
        processMatches(matches, firstIteration);
        firstIteration = false;
    } while (!matches.empty());

    return true;
}

void Board::handleClick(int x, int y) {
    int i = y / GameConfig::CELL_SIZE;
    int j = x / GameConfig::CELL_SIZE;

    if (!isValid(i, j)) return;

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

    trySwap(i, j);

    firstSelected = false;
}

void Board::draw(sf::RenderWindow& window) {
    for (int i = 0; i < GameConfig::GRID_SIZE; i++) {
        for (int j = 0; j < GameConfig::GRID_SIZE; j++) {
            if (!grid[i][j]) continue;
            grid[i][j]->draw(window, colors, GameConfig::CELL_SIZE,
                j * GameConfig::CELL_SIZE, i * GameConfig::CELL_SIZE);
        }
    }
