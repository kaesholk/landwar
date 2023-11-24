#include "game.h"
#include <iostream>
#include <algorithm>
#include <random>
#include <fstream>
#include <cmath>

using namespace std;

typedef std::pair<size_t, size_t> MapIndex;

void Game::read_config(string filename) {
    ifstream ifs{filename};
    
    if(!ifs.is_open()) {
        cout << "Failed to open config file, using default settings.\n";
        return;
    }

    cfg = Config{};
    string junk;
    ifs >> junk >> cfg.map_width;
    if(cfg.map_width > 52) {
        cout << "map width cannot be greater than 52\n";
        exit(1);
    }

    ifs >> junk >> cfg.map_height;

    long random_seed_tmp;
    ifs >> junk >> random_seed_tmp;
    if(random_seed_tmp != -1)
        cfg.random_seed = random_seed_tmp;

    ifs >> junk >> cfg.max_ally_tiles;
    ifs >> junk >> cfg.max_nuke_tiles;
    ifs >> junk >> cfg.max_revolt_tiles;
    ifs >> junk >> cfg.max_spy_tiles;
    ifs >> junk >> cfg.num_player_actions;
}

void Game::fill_map() {
    map.resize(cfg.map_height, vector<Tile>(cfg.map_width));

    vector<TileType> tile_types = {TileType::Ally, TileType::Revolt, 
                                   TileType::Spy, TileType::Nuke};
    
    // generate all possible pairs of indices
    std::vector<MapIndex> all_indices;
    for (size_t i = 0; i < cfg.map_height; ++i) {
        for (size_t j = 0; j < cfg.map_width; ++j) {
            all_indices.push_back(MapIndex(i, j));
        }
    }
    std::shuffle(all_indices.begin(), all_indices.end(), 
                 std::default_random_engine(cfg.random_seed));
    auto it = all_indices.begin();

    std::map<TileType, size_t> TILE_COUNTS = {
        { TileType::Ally,   cfg.max_ally_tiles},
        { TileType::Nuke,   cfg.max_nuke_tiles},
        { TileType::Revolt, cfg.max_revolt_tiles},
        { TileType::Spy,    cfg.max_spy_tiles},
    };

    for(TileType T : tile_types) {
        for(size_t i = 0; i < TILE_COUNTS[T]; i++) {
            
            // if the grid is filled, just stop
            if(it == all_indices.end()) break;
            
            size_t row = it->first;
            size_t col = it->second;
            map[row][col].tile_type = T;
            it++;
        }
    }
}

void Game::print_map() {
    // used to calculate max space length
    int max_label_digits = static_cast<int>(log10(cfg.map_height)) + 1;

    // print letters for column labels
    string space(max_label_digits, ' ');
    cout << space;
    for (size_t j = 0; j < cfg.map_width; ++j) {
        if (j < 26)
            cout << ' ' << char('A' + j);
        else
            cout << ' ' << char('a' + (j-26));
    }
    cout << '\n';

    // print numbers for row labels
    for (size_t i = 0; i < cfg.map_height; ++i) {
        // determine correct spacing, account for log10(0) using ternary
        int curr_label_digits = i == 0 ? 0 : static_cast<int>(log10(i));
        int space_length = max_label_digits - curr_label_digits - 1;
        string space(space_length, ' ');
        cout << i << space;
        
        for (size_t j = 0; j < cfg.map_width; ++j)
            cout << ' ' << map[i][j];
        
        cout << '\n';
    }   
}

void Game::play() {
    fill_map();

    cout << "Welcome to Land War in Southeast Latvia: The Video Game!\n";
    cout << "The year is 1973. Genesis has just released their masterpiece Selling England by the Pound, and the Soviet Union has launched a land invasion of the Republic of Latvia, which gained its independence after World War II. Will democracy prevail or will the once-free Republic of Latvia fall under the Soviet stronghold? A thrilling game of survival and strategy awaits - prepare to shape history.\n";
    cout << "Player 1: Soviet Union\n";
    cout << "Player 2: Republic of Latvia\n";

    while(true) {
        take_turn(TileStatus::Player1);
        take_turn(TileStatus::Player2);
    }
}

void Game::take_turn(TileStatus player) {
    int num_actions = cfg.num_player_actions;
    while(num_actions > 0) {
        cout << "\n";
        print_map();
        cout << "Player " << static_cast<int>(player) << "'s turn, " 
             << num_actions << " actions remaining. Input an action: ";
        if(take_action(player)) num_actions--;
    }
}

size_t Game::convert_x(char x_char) {
    if(x_char < 'a')
        return x_char - 'A';
    else
        return x_char - 'a';
}

bool Game::take_action(TileStatus player) { 
    string command;
    cin >> command;
    char firstchar = command[0];
    size_t y;
    char x_char;

    switch(firstchar) {
        case 's':
            cin >> x_char >> y;
            return scout(convert_x(x_char), y);
            break;
        
        case 'o':
            cin >> x_char >> y;
            return occupy(player, convert_x(x_char), y);
            break;

        case 'a':
            cin >> x_char >> y;
            return annex(player, convert_x(x_char), y);
            break;

        default:
            cout << "Invalid command. Input an action: ";
            return false;
            break;
    }
}

bool Game::scout(size_t x, size_t y) {
    Tile T = map[x][y];

    if(T.status != TileStatus::Unclaimed) {
        cout << "Cannot scout a claimed tile. Input an action: ";
        return false;
    }

    cout << "Tile type revealed: " << TILE_CHARS[T.tile_type] << '\n';
    return true;
}

bool Game::occupy(TileStatus player, size_t x, size_t y) {
    (void)player;
    (void)x;
    (void)y;
    cout << "occupy placeholder\n";
    return false;
}

bool Game::annex(TileStatus player, size_t x, size_t y) {
    (void)player;
    (void)x;
    (void)y;
    cout << "annex placeholder\n";
    return false;
}