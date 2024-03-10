#pragma once

#include <vector>
#include "options.h"
#include "tile.h"
#include <ctime>
#include <random>

struct Config {
    size_t map_width = 8;
    size_t map_height = 8;
    ulong random_seed = time(0);
    int max_ally_tiles = 4;
    int max_revolt_tiles = 4;
    int max_spy_tiles = 4;
    int max_nuke_tiles = 4;
    int num_player_actions = 2;
};

struct MapIndex {
    int x;
    int y;
};

const std::vector<MapIndex> ADJACENT{{-1,1},  {0,1},  {1,1},
                                     {-1,0},          {1,0},
                                     {-1,-1}, {0,-1}, {1,-1}};

const std::vector<MapIndex> ORTHOGONAL{        {0,1},
                                       {-1,0},         {1,0},
                                               {0,-1},       };

struct pair_hash {
    inline std::size_t operator()(const std::pair<int, int> & v) const {
        return v.first*31+v.second;
    }
};

class Game {
    public:
        Game(Options* opts_in) : opts{opts_in} {
            read_config("config.txt");
            rng = std::default_random_engine{cfg.random_seed};
            num_neutral_tiles = static_cast<int>(cfg.map_height) - 1 * 
                                static_cast<int>(cfg.map_width - 1);
        }
        void play();

    private:
        Options* opts;
        Config cfg;
        std::vector<std::vector<Tile>> map;
        std::default_random_engine rng;
        int num_neutral_tiles;

        void read_config(std::string filename);
        void fill_map();
        void print_map();
        void take_turn(TileStatus player);
        int col_to_int(char col);
        std::vector<MapIndex> get_friendly_neighbors(TileStatus player, size_t x, size_t y);
        std::vector<MapIndex> get_neighbors(std::vector<MapIndex> directions, size_t x, size_t y);
        
        int count_friendly_neighbors(TileStatus player, size_t x, size_t y) {
            return static_cast<int>(get_friendly_neighbors(player, x, y).size());
        }

        bool is_in_bounds(int x, int y) {
            return (x >= 0 && x < static_cast<int>(cfg.map_width) 
                 && y >= 0 && y < static_cast<int>(cfg.map_height));
        }
        
        TileStatus other_player(TileStatus current) {
            switch (current) {
                case TileStatus::Player1:
                    return TileStatus::Player2;
                case TileStatus::Player2:
                    return TileStatus::Player1;
                default:
                    throw std::invalid_argument("Not a player");
            }
        }
        
        // actions return false if there is an error, so the player can try again
        void take_action(TileStatus player);
        bool scout(size_t x, size_t y);
        bool capture(TileStatus player, size_t x, size_t y);
        bool annex(TileStatus player, size_t x, size_t y);

        // tile effects
        void effect_ally(TileStatus player);
        void effect_nuke(size_t x, size_t y);
        void effect_revolt(size_t x, size_t y);
        void effect_spy(size_t x, size_t y, TileStatus player);

        void end_game();
};