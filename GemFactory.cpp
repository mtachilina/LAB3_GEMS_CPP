#include "GemFactory.hpp"

std::unique_ptr<Gem> GemFactory::createGem(int color, int x, int y, GemType type) {
    switch (type) {
    case GemType::Bomb:
        return std::make_unique<BombGem>(color, x, y);
    case GemType::Color:
        return std::make_unique<ColorGem>(color, x, y);
    default:
        return std::make_unique<KeeperGem>(color, x, y);
    }
}
