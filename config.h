#pragma once

#include "json.hpp"
#include <fstream>
#include <iostream>

class ConfigFile {
    public:
        ConfigFile(const std::string& filePath) {
            std::ifstream file(filePath);
            if (file.is_open()) {
                file >> j;
                file.close();
            }
            else
                std::cout << "Couldn't open config file for reading: " << filePath << std::endl;

            read_params();
        }

        size_t get_map_width() {return map_width;}
        size_t get_map_height() {return map_height;}
        long get_random_seed() {return random_seed;}
        size_t get_max_ally_tiles() {return max_ally_tiles;}
        size_t get_max_revolt_tiles() {return max_revolt_tiles;}
        size_t get_max_spy_tiles() {return max_spy_tiles;}
        size_t get_max_nuke_tiles() {return max_nuke_tiles;}

    private:
        nlohmann::json j;

        template <typename T>
        T get_param(const std::string& key, const T& defaultValue) {
            auto it = j.find(key);
            if (it != j.end() && !it->is_null()) {
                return it->get<T>();
            }
            return defaultValue;
        }

        void read_params();

        size_t map_width;
        size_t map_height;
        long random_seed;
        size_t max_ally_tiles;
        size_t max_revolt_tiles;
        size_t max_spy_tiles;
        size_t max_nuke_tiles;
};
