#ifndef ENGINE_H
#define ENGINE_H
#include <engine/lamps/tuyacontroller.h>


class Engine
{
public:
    Engine();
    std::vector<Lamp> getLamps();

private:
    TuyaController tuyaController;
    std::vector<Lamp> lamps;
};

#endif // ENGINE_H
