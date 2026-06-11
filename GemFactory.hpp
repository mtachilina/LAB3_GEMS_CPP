#pragma once
#include <memory>
#include "KeeperGem.hpp"
#include "BombGem.hpp"
#include "ColorGem.hpp"

enum class GemType {
    Keeper,
    Bomb,
    Color
};

class GemFactory {
public:
    static std::unique_ptr<Gem> createGem(int color, int x, int y, GemType type);
};
