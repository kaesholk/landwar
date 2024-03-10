#pragma once

#include <iostream>
#include <map>
#include "colors.h"

enum class TileType {
    Blank,
    Ally,
    Nuke,
    Revolt,
    Spy,
};
extern std::map<TileType, char> TILE_CHARS;
extern std::map<TileType, size_t> TILE_COUNTS;

enum class TileStatus {
    Neutral,
    Player1,
    Player2,
    Nuked
};

struct Tile {
    TileType tile_type = TileType::Blank;
    TileStatus status = TileStatus::Neutral;
    bool discovered = false;
};

std::ostream& operator<<(std::ostream& os, const Tile& tile);