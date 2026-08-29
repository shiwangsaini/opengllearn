#include "Core/GameWindow.h"
//#include <iostream>

int main(void)
{
    GameWindow game(1280, 720, "Game");

    if (!game.init()) {
        std::cout << "could not run the Game\n";
        return -1;
    }

    game.run();

    game.shutdown();
}