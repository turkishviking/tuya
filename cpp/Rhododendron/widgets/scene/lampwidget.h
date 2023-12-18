#ifndef LAMPWIDGET_H
#define LAMPWIDGET_H

#include <QWidget>
#include <QLabel>
#include <engine/lamps/lamp.h>

#include <widgets/scene/abstractscenewidget.h>
#include <QVBoxLayout>

class LampWidget : public AbstractSceneWidget
{
    Q_OBJECT
public:
    explicit LampWidget(QWidget *parent = nullptr, Lamp *lamp = nullptr);
    QString getId();
    QString getName();
    Lamp* getLamp();

private:
    Lamp lamp;
    QLabel label;
    QVBoxLayout layout;
signals:

};

#endif // LAMPWIDGET_H
