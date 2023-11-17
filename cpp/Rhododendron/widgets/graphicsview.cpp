#include "graphicsview.h"

GraphicsView::GraphicsView(QWidget *parent) : QGraphicsView(parent)
{
    setTransformationAnchor(QGraphicsView::NoAnchor);
}

