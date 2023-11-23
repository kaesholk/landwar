#include "game.h"
#include <iostream>
#include <algorithm>
#include <random>
#include <fstream>

using namespace std;

typedef std::pair<size_t, size_t> MapIndex;

void Game::fill_map() {
    map.resize(cfg->get_map_width(), vector<Tile>(cfg->get_map_height()));

    vector<TileType> tile_types = {TileType::Ally, TileType::Revolt, 
                                   TileType::Spy, TileType::Nuke};
    
    // generate all possible pairs of indices
    std::vector<MapIndex> all_indices;
    for (size_t i = 0; i < cfg->get_map_height(); ++i) {
        for (size_t j = 0; j < cfg->get_map_width(); ++j) {
            all_indices.push_back(MapIndex(i, j));
        }
    }
    std::shuffle(all_indices.begin(), all_indices.end(), 
                 std::default_random_engine(cfg->get_random_seed()));
    auto it = all_indices.begin();

    std::map<TileType, size_t> TILE_COUNTS = {
        { TileType::Ally,   cfg->get_max_ally_tiles()},
        { TileType::Revolt, cfg->get_max_revolt_tiles()},
        { TileType::Spy,    cfg->get_max_spy_tiles()},
        { TileType::Nuke,   cfg->get_max_nuke_tiles()},
    };

    for(TileType T : tile_types) {
        for(size_t i = 0; i < TILE_COUNTS[T]; i++) {
            size_t row = it->first;
            size_t col = it->second;
            map[row][col].tile_type = T;
            it++;
        }
    }
}

void Game::print_map() {
    cout << "  ";
    for (size_t j = 0; j < cfg->get_map_width(); ++j) 
    {
        if (j < 26)
            cout << ' ' << char('A' + j);
        else
            cout << ' ' << char('a' + j);
    }
    std::cout << '\n';

    for (size_t i = 0; i < cfg->get_map_height(); ++i)
    {
        std::cout << i + 1 << ' ';
        for (size_t j = 0; j < cfg->get_map_height(); ++j)
        {
            std::cout << ' ' << map[i][j];
        }
        std::cout << '\n';
    }   
}

void Game::play() {
    fill_map();
    print_map();
}