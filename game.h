#pragma once

#include <vector>
#include "options.h"
#include "tile.h"
#include <ctime>

struct Config {
    size_t map_width = 7;
    size_t map_height = 7;
    long random_seed = time(0);
    int max_ally_tiles = 4;
    int max_revolt_tiles = 4;
    int max_spy_tiles = 4;
    int max_nuke_tiles = 4;
};

class Game {
    public:
        Game(Options* opts_in) : opts{opts_in} {
            read_config("config.txt");
        }
        void play();

    private:
        Options* opts;
        Config cfg;
        std::vector<std::vector<Tile>> map;

        void read_config(std::string filename);
        void fill_map();
        void print_map();
};