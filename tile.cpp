#include "tile.h"

std::map<TileType, char> TILE_CHARS = {
    { TileType::Blank,  '.' },
    { TileType::Ally,   'A' },
    { TileType::Revolt, 'R' },
    { TileType::Spy,    'S' },
    { TileType::Nuke,   'N' },
};

std::ostream& operator<<(std::ostream& os, const Tile& tile) {
    // set correct color (default is white for unclaimed)
    if(tile.status == TileStatus::Player1)
        std::cout << Colors::red;
    else if(tile.status == TileStatus::Player2)
        std::cout << Colors::blue;

    if(tile.status == TileStatus::Unclaimed)
        std::cout << TILE_CHARS[TileType::Blank];
    else
        std::cout << TILE_CHARS[tile.tile_type];
    
    std::cout << Colors::reset;

    return os;
}