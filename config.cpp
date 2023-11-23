#include "config.h"
#include <ctime>

void ConfigFile::read_params() {
    map_width = get_param("map_width", 7);
    map_height = get_param("map_height", 7);
    random_seed = get_param("random_seed", time(0));
    max_ally_tiles = get_param("max_ally_tiles", 4);
    max_revolt_tiles = get_param("max_revolt_tiles", 4);
    max_spy_tiles = get_param("max_spy_tiles", 4);
    max_nuke_tiles = get_param("max_nuke_tiles", 4);
}