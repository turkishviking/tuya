#include "engine.h"

Engine::Engine()
{
    lamps = tuyaController.getLamps();
}

std::vector<Lamp> Engine::getLamps()
{
    return lamps;
}
