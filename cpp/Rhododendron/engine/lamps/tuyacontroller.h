#ifndef TUYACONTROLLER_H
#define TUYACONTROLLER_H
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>

class TuyaController
{
public:
    TuyaController();
private:
    QTcpSocket *socket;
};

#endif // TUYACONTROLLER_H
