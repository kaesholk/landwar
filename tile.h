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
    Unclaimed,
    Player1,
    Player2
};

struct Tile {
    TileType tile_type;
    TileStatus status;
    
    Tile() : tile_type{TileType::Blank}, status{TileStatus::Unclaimed} {}
};

std::ostream& operator<<(std::ostream& os, const Tile& tile);