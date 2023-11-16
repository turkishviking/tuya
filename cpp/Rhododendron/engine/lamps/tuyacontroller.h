#ifndef TUYACONTROLLER_H
#define TUYACONTROLLER_H
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include <engine/lamps/lamp.h>
#include <QJsonArray>

class TuyaController
{
public:
    TuyaController();
    std::vector<Lamp> getLamps();
private:
    QTcpSocket *socket;
    std::vector<Lamp> lamps;
};

#endif // TUYACONTROLLER_H
