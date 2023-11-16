#include "tuyacontroller.h"

TuyaController::TuyaController()
{
    socket = new QTcpSocket;
    socket->connectToHost("127.0.0.1", 65432);

    if(socket->waitForConnected(3000))
    {
        //qDebug() << "Connected!";
        // send
        QJsonObject json;
        QJsonDocument jsonDoc;
        json.insert("cmd", "getLamps");
        jsonDoc.setObject(json);

        socket->write(jsonDoc.toJson());
        socket->waitForBytesWritten();
        socket->waitForReadyRead();
        //qDebug() << "Reading: " << socket->bytesAvailable();
        QJsonDocument res = QJsonDocument::fromJson(socket->readAll());

        int i = 1;
        foreach (const QJsonValue & v, res.array())
        {
            Lamp l;
            l.id = v.toObject().value("id").toString();
            l.name = QString("tuya ") + QString::number(i);
            lamps.push_back(l);
            i++;
        }

        socket->close();
    }
    else
    {
        qDebug() << "Not connected!";
    }

}

std::vector<Lamp> TuyaController::getLamps()
{
    return lamps;
}
