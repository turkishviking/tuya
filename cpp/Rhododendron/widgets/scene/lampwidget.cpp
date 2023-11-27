#include "lampwidget.h"

LampWidget::LampWidget(QWidget *parent, Lamp *l)
    :
    AbstractSceneWidget{parent}
{

    layout.setContentsMargins(QMargins(0,0,0,0));
    layout.setAlignment(Qt::AlignCenter);
    this->setLayout(&layout);
    layout.addWidget(&label);

    label.setText(l->name);
    label.setMinimumSize(QSize(48,48));
    label.setAlignment(Qt::AlignCenter);


    this->setMinimumSize(QSize(48,48));
    this->setVisible(true);
    this->setCursor(Qt::ClosedHandCursor);

    lamp = *l;

    setDatas("type", "LampWidget");
    setDatas("id", lamp.id.toUtf8());
    setDatas("name", lamp.name.toUtf8());

}


QString LampWidget::getId()
{
    return lamp.id;
}


QString LampWidget::getName()
{
    return lamp.name;
}
