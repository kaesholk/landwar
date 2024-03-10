# landwar
landwar, or "Land War in Southeast Latvia," is an adaptation of a card game that myself and two friends created a couple of years ago at a summer camp. It's a two player strategy game where each player tries to capture as much territory on the board as possible, by capturing tiles one at a time, or annexing surrounded areas, similar to Go. A few tiles have special effects that activate when captured, leading to wacky hijinks. The game ends when all territory has been captured.

## configuration
You can modify the config.txt file to control various aspects of the game.
```
map_width, map_height (size of the map, the edges start off already controlled, default: 8)
random_seed (seed for the random number generator)
max_ally_tiles, max_revolt_tiles, max_spy_tiles, max_nuke_tiles (number of special tiles of each type, default: 4)
num_player_actions (how many actions the player may take each turn, default: 2)
```

## gameplay
Actions are input as follows. Columns are represented by letters, rows as numbers, e.g. `c E 4`
- capture a tile: `c <col> <row>`
- scout a tile: `s <col> <row>`
- annex a region: `a <col> <row>` (select any tile within the surrounded area)

Tile effects:
- aid from ally: gain an extra action.
- revolt: choose any adjacent captured tile to return to neutrality.
- spy: the opponent chooses any adjacent enemy tile to take control of themselves.
- nuke: the tile is destroyed and can no longer be occupied.

The revolt and spy effects prompt one of the players to select a specific tile, just input the row and column in the same format.
