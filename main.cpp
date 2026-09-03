#include "Core/GameWindow.h"
//#include <iostream>

int main(void)
{
    GameWindow game(1280, 720, "Game");

    try {
        game.init();
        game.run();
    }
    catch (const char* e) {
        std::cout << e ;
    }
    game.shutdown();
}