#pragma once

#include <vector>
#include "options.h"
#include "tile.h"
#include "config.h"

class Game {
    public:
        Game(Options* opts_in, ConfigFile* cfg_in) : opts{opts_in}, cfg{cfg_in} {}
        void play();

    private:
        Options* opts;
        ConfigFile* cfg;
        std::vector<std::vector<Tile>> map;

        void fill_map();
        void print_map();
};