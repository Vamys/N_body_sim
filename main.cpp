
#include <iostream>
#include <vector>
#include "engine.h"
//INSTRUCTIONS:
// SPACE: PAUSE - START
// Q: SHOW- QUAD TREE
int main()
{
    engine Engine = engine();
    if (0 != Engine.init()){
        throw std::runtime_error("Failed to initialize engine");
    }
    else{
        std::cout << "Engine initialized" << std::endl;
    }
    Engine.run();
    return 0;
}