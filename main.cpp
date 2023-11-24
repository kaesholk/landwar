#include "game.h"
#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
    //ios_base::sync_with_stdio(false);

    Options opts{};
    opts.handle_input(argc, argv);
    Game game{&opts};
    game.play();
}