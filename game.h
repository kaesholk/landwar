#pragma once

#include <vector>
#include "options.h"
#include "tile.h"
#include <ctime>
#include <random>

struct Config {
    size_t map_width = 7;
    size_t map_height = 7;
    ulong random_seed = time(0);
    int max_ally_tiles = 4;
    int max_revolt_tiles = 4;
    int max_spy_tiles = 4;
    int max_nuke_tiles = 4;
    int num_player_actions = 2;
};

class Game {
    public:
        Game(Options* opts_in) : opts{opts_in} {
            read_config("config.txt");
            rng = std::default_random_engine{cfg.random_seed};
        }
        void play();

    private:
        Options* opts;
        Config cfg;
        std::vector<std::vector<Tile>> map;
        std::default_random_engine rng;

        void read_config(std::string filename);
        void fill_map();
        void print_map();
        void take_turn(TileStatus player);
        size_t convert_x(char x_char);
        vector<pair<int, int>> get_friendly_adjacencies(TileStatus player, size_t x, size_t y);
        
        int count_friendly_adjacencies(TileStatus player, int x, int y) {
            return get_friendly_adjacencies(player, x, y).size();
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
        bool take_action(TileStatus player);
        bool scout(size_t x, size_t y);
        bool capture(TileStatus player, size_t x, size_t y);
        bool annex(TileStatus player, size_t x, size_t y);
};