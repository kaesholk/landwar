#include "game.h"

using namespace std;

int main(int argc, char *argv[]) {
    Options opts{};
    opts.handle_input(argc, argv);

    ConfigFile cfg{"config.json"};

    Game game{&opts, &cfg};
    game.play();
}