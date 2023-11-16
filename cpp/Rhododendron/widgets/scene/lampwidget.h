#ifndef LAMPWIDGET_H
#define LAMPWIDGET_H

#include <QWidget>
#include <QToolButton>
#include <engine/lamps/lamp.h>
#include <QMouseEvent>
#include <QPointF>
#include <QApplication>
#include <QDrag>
#include <QMimeData>

class LampWidget : public QToolButton
{
    Q_OBJECT
public:
    explicit LampWidget(QWidget *parent = nullptr, Lamp *lamp = nullptr);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private:
    Lamp lamp;
    QPointF dragStartPosition;
signals:

};

#endif // LAMPWIDGET_H
