#include "game.h"
#include <iostream>
#include <algorithm>
#include <random>
#include <fstream>
#include <cmath>
#include <unordered_set>
#include <queue>

using namespace std;

void Game::read_config(string filename) {
    ifstream ifs{filename};
    
    if(!ifs.is_open()) {
        cout << "Failed to open config file, using default settings.\n";
        return;
    }

    cfg = Config{};
    string junk;

    ifs >> junk >> cfg.map_width;
    ifs >> junk >> cfg.map_height;
    if(cfg.map_width > 52) {
        cout << "map width cannot be greater than 52\n";
        exit(1);
    }

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
    
    // generate all possible pairs of indices (inside the edges)
    std::vector<MapIndex> all_indices;
    for (size_t i = 1; i < cfg.map_height-1; ++i) {
        for (size_t j = 1; j < cfg.map_width-1; ++j) {
            all_indices.push_back({static_cast<int>(i), static_cast<int>(j)});
        }
    }
    std::shuffle(all_indices.begin(), all_indices.end(), rng);
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
            
            int row = it->x;
            int col = it->y;
            map[row][col].tile_type = T;
            it++;
        }
    }

    // set starting territory for both players
    // left & right edges
    for (size_t i = 0; i < cfg.map_height; ++i) {
        map[i][0].status = TileStatus::Player1;
        map[i][cfg.map_width-1].status = TileStatus::Player2;
    }
    // top & bottom edges
    for (size_t i = 0; i < cfg.map_width; ++i) {
        map[0][i].status = TileStatus::Player1;
        map[cfg.map_height-1][i].status = TileStatus::Player2;
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
    cout << "The year is 1973. Genesis has just released their masterpiece \"Selling England by the Pound\", and the Soviet Union has launched a land invasion of the Republic of Latvia, which gained its independence after World War II. Will democracy prevail or will the once-free Republic of Latvia fall under the Soviet stronghold? A thrilling game of survival and strategy awaits - prepare to shape history.\n";
    cout << "Player 1: Soviet Union (red)\n";
    cout << "Player 2: Republic of Latvia (blue)\n";

    while(true) {
        if(num_neutral_tiles <= 0)
            end_game();
        take_turn(TileStatus::Player1);
        
        if(num_neutral_tiles <= 0)
            end_game();
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
        take_action(player);
        num_actions--;
    }
}

int Game::col_to_int(char col) {
    if(col < 'a')
        return col - 'A';
    else
        return col - 'a';
}

void Game::take_action(TileStatus player) { 
    bool success = false;
    while(!success) {
        cin.clear();
        string command;
        cin >> command;
        char firstchar = command[0];

        char col;
        size_t row;
        cin >> col >> row;

        if(!is_in_bounds(col_to_int(col), static_cast<int>(row))) {
            cout << "Invalid command: out of bounds. Input an action: ";
            continue;
        }

        switch(firstchar) {
            case 's':
                success = scout(col_to_int(col), row);
                break;
            
            case 'c':
                success = capture(player, col_to_int(col), row);
                break;

            case 'a':
                success = annex(player, col_to_int(col), row);
                break;

            default:
                cout << "Invalid command. Input an action: ";
                break;
        }
    }
}

bool Game::scout(size_t x, size_t y) {
    Tile T = map[x][y];

    if(T.status != TileStatus::Neutral) {
        cout << "Invalid action: cannot scout a claimed tile.\n";
        cout << "Input an action: ";
        return false;
    }

    cout << "Tile type revealed: " << TILE_CHARS[T.tile_type] << '\n';
    return true;
}

bool Game::capture(TileStatus player, size_t x, size_t y) {
    // check if tile is valid for capture
    Tile& T = map[x][y];
    if(T.status == player) {
        cout << "Invalid action: cannot capture your own tile.\n";
        cout << "Input an action: ";
        return false;
    }
    if(count_friendly_neighbors(player, x, y) < 1) {
        cout << "Invalid action: tile must be adjacent to a friendly tile.\n";
        cout << "Input an action: ";
        return false;
    }
    // fight opponent
    if(T.status != TileStatus::Neutral) {
        uniform_int_distribution<int> die_roll{1,6}; 
        int attacker_roll = die_roll(rng) + count_friendly_neighbors(player, x, y);
        int defender_roll = die_roll(rng) + count_friendly_neighbors(other_player(player), x, y);
        cout << "Combat: attacker rolled " << attacker_roll << ", defender rolled " << defender_roll << ".\n"; 

        if(attacker_roll < defender_roll) {
            cout << "Attacker lost, no territory was captured.\n";
            return true;
        } else {
            cout << "Attacker won, territory captured.\n";
            T.status = player;
            return true;
        }
    }

    // activate tile effect
    switch(T.tile_type) {
        case TileType::Ally:
            T.status = player;
            print_map();
            effect_ally(player);
            break;

        case TileType::Nuke:
            effect_nuke(x, y);
            break;

        case TileType::Revolt:
            effect_revolt(x, y);
            break;

        case TileType::Spy:
            effect_spy(x, y, player);
            break;

        default:
            break;
    }

    num_neutral_tiles--;
    T.status = player;
    return true;
}

bool Game::annex(TileStatus player, size_t x, size_t y) {
    unordered_set<pair<int, int>, pair_hash> discovered;
    queue<MapIndex> q;
    q.push(MapIndex{static_cast<int>(x), static_cast<int>(y)});

    while(!q.empty()) {
        MapIndex idx = q.front();
        q.pop();

        // check if still within bounds
        if(!is_in_bounds(idx.x, idx.y)) {
            cout << "Invalid action: region is not completely surrounded\n";
            cout << "Input an action: ";
            return false;
        }

        // only discover blank or enemy tiles, anything friendly is probably a border
        if(map[idx.x][idx.y].status != player && discovered.find({idx.x, idx.y}) != discovered.end()) {
            discovered.insert({idx.x, idx.y});
            vector<MapIndex> neighbors = get_neighbors(ORTHOGONAL, idx.x, idx.y);
            for(MapIndex neighbor : neighbors)
                q.push(neighbor);
        }
    }

    for(pair<int,int> idx : discovered) {
        if(map[idx.first][idx.second].status == TileStatus::Neutral)
            num_neutral_tiles--;
        map[idx.first][idx.second].status = player;
    }

    cout << "Territory annexed.\n";
    return true;
}

vector<MapIndex> Game::get_friendly_neighbors(TileStatus player, size_t x, size_t y) {
    vector<MapIndex> neighbors;
    for(auto d : ADJACENT) {
        int x_d = d.x + static_cast<int>(x);
        int y_d = d.y + static_cast<int>(y);
        if(!is_in_bounds(x_d, y_d))
            continue;

        if(map[x_d][y_d].status == player)
            neighbors.push_back({x_d, y_d});
    }

    return neighbors;
}

vector<MapIndex> Game::get_neighbors(vector<MapIndex> directions, size_t x, size_t y) {
    vector<MapIndex> neighbors;
    for(auto d : directions) {
        int x_d = d.x + static_cast<int>(x);
        int y_d = d.y + static_cast<int>(y);
        if(!is_in_bounds(x_d, y_d))
            continue;
    }
    return neighbors;
}

void Game::effect_ally(TileStatus player) {
    cout << "Recieved aid from an ally. Take a free action.";
    take_action(player);
}

void Game::effect_nuke(size_t x, size_t y) {
    cout << "A nuke destroyed the tile. \n";
    map[x][y].status = TileStatus::Nuked;
}

void Game::effect_revolt(size_t x, size_t y) {
    cout << "Revolt: choose any adjacent tile to return to neutrality (valid tiles: ";

    unordered_set<pair<int, int>, pair_hash> valid_tiles;
    for(auto d : ADJACENT) {
        int x_d = d.x + static_cast<int>(x);
        int y_d = d.y + static_cast<int>(y);
        if(map[x_d][y_d].status != TileStatus::Neutral) {
            cout << '(' << static_cast<char>(x + d.x + 'A') << ", " << y + d.y << ')';
            valid_tiles.insert({x_d, y_d});
        }
    }

    char input_x;
    int input_y;
    cin >> input_x >> input_y;
    int input_x_num = col_to_int(input_x);
    while(valid_tiles.find({input_x_num, input_y}) == valid_tiles.end()) {
        cout << "Invalid tile, choose any adjacent occupied tile: ";
    }
    map[input_x_num][input_y].status = TileStatus::Neutral;
    map[input_x_num][input_y].discovered = false;
    num_neutral_tiles++;
}

void Game::effect_spy(size_t x, size_t y, TileStatus player) {
    cout << "An enemy spy allowed the enemy to take over one of your tiles. \n";
    cout << "Opponent: Choose an adjacent enemy tile to occupy (valid tiles: ";

    unordered_set<pair<int, int>, pair_hash> valid_tiles;
    for(auto d : ADJACENT) {
        int x_d = d.x + static_cast<int>(x);
        int y_d = d.y + static_cast<int>(y);
        if (map[x_d][y_d].status == player) {
            cout << '(' << static_cast<char>(x + d.x + 'A') << ", " << y + d.y << ')';
            valid_tiles.insert({x_d, y_d});
        }
    }

    char input_x;
    int input_y;
    cin >> input_x >> input_y;
    int input_x_num = col_to_int(input_x);
    while(valid_tiles.find({input_x_num, input_y}) == valid_tiles.end()) {
        cout << "Invalid tile, choose any adjacent enemy tile: ";
    }
    map[input_x_num][input_y].status = other_player(player);
}
     
void Game::end_game() {
    // count territory
    int player_1_tiles = 0;
    int player_2_tiles = 0;
    for (size_t i = 0; i < cfg.map_height; ++i) {
        for (size_t j = 0; j < cfg.map_width; ++j) {
            if(map[i][j].status == TileStatus::Player1)
                player_1_tiles++;
            else if(map[i][j].status == TileStatus::Player2)
                player_2_tiles++;
        }
    }   

    if(player_1_tiles > player_2_tiles) {
        cout << "Game over! Player 1 wins, with " << player_1_tiles << " tiles captured.\n";
    } else if (player_2_tiles > player_1_tiles) {
        cout << "Game over! Player 2 wins, with " << player_2_tiles << " tiles captured.\n";
    } else {
        cout << "Game over! Draw.\n";
    }
    exit(0);
}