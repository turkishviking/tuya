#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QGraphicsView>
#include <QMouseEvent>

class GraphicsView : public QGraphicsView
{
public:
    GraphicsView(QWidget *parent);


private:
    QPoint posOrigin;
};

#endif // GRAPHICSVIEW_H
