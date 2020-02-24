#include <iostream>
#include <windows.h>
#include <ctime>
#include <cmath>

#include "../../ImasiEngine/Source/Engine/Engine.hpp"

#include "Scenes/MenuScene.hpp"

int main()
{
    srand(time(NULL));

    ImasiEngine::Engine engine;
    engine.setupWindow("Galactic Offensive", 4U, 500,500);
    engine.run(new MenuScene());

    if (engine.getWindow() != nullptr)
    {
        engine.getWindow()->close();
    }

    std::cout << "Press a key to exit..." << std::endl;
    std::cin.get();

    return 0;
}
