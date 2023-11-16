#ifndef ENGINE_H
#define ENGINE_H
#include <engine/lamps/tuyacontroller.h>

class Engine
{
public:
    Engine();

private:
    TuyaController tuyaController;
};

#endif // ENGINE_H
