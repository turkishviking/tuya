#ifndef ENGINE_H
#define ENGINE_H
#include "widgets/scene/groupwidget.h"
#include <engine/lamps/tuyacontroller.h>



class Engine
{
public:
    Engine();
    std::vector<Lamp> getLamps();
    GroupWidget* groupWidget;


private:
    TuyaController tuyaController;
    std::vector<Lamp> lamps;
};

#endif // ENGINE_H
