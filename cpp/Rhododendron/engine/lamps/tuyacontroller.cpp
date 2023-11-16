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
        qDebug() << socket->readAll();

        socket->close();
    }
    else
    {
        qDebug() << "Not connected!";
    }

}
