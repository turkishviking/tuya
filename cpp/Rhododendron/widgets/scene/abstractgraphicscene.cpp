#include "abstractgraphicscene.h"

AbstractGraphicScene::AbstractGraphicScene(QObject *parent)
    : QGraphicsScene{parent}
{
    QBrush penBrush(QColor(200,200,200, 150), Qt::BrushStyle::SolidPattern);
    QPen pen(penBrush, 2, Qt::PenStyle::SolidLine);
    rectSelection.setPen(pen);

    QBrush rectBrush(QColor(240,148,239, 50), Qt::BrushStyle::SolidPattern);
    rectSelection.setBrush(rectBrush);
    rectSelection.setZValue(1000);
    addItem(&rectSelection);

    addItem(&groupedSelection);
    groupedSelection.setFlag(QGraphicsItem::ItemIsMovable);
}


QGraphicsProxyWidget* AbstractGraphicScene::toProxy(QGraphicsItem* item)
{
    QGraphicsProxyWidget* proxy = static_cast<QGraphicsProxyWidget*>(item);
    return proxy;
}


void AbstractGraphicScene::selectItem(QGraphicsProxyWidget* item)
{
    groupedSelection.addToGroup(item);
    auto abstractWidget = static_cast<AbstractSceneWidget*>(item->widget());
    abstractWidget->select();
    update();
}


void AbstractGraphicScene::unSelectItem(QGraphicsProxyWidget* item)
{
    groupedSelection.removeFromGroup(item);
    auto abstractWidget = static_cast<AbstractSceneWidget*>(item->widget());
    abstractWidget->unSelect();
    update();
}


void AbstractGraphicScene::UnselectAll()
{
    for(auto &item : groupedSelection.childItems())
    {
        unSelectItem(toProxy(item));
    }
}


void AbstractGraphicScene::removeSelectedFromScene()
{
    for(auto &item : groupedSelection.childItems())
    {
        qDebug() << "s" << item << "\n";
        removeItem(item);
        auto proxy = toProxy(item);

        emit itemRemoved(proxy);
        proxy->setWidget(nullptr);
    }
    update();
}


void AbstractGraphicScene::keyPressEvent(QKeyEvent * event)
{
    //qDebug() << event->key();
    if(event->key() == Qt::Key_Control)
    {
        isCtrlKey = true;
        event->accept();
    }
    else if(event->key() == Qt::Key_Delete || event->key() == Qt::Key_Backspace)
    {
        removeSelectedFromScene();
    }
}


void AbstractGraphicScene::keyReleaseEvent(QKeyEvent * event)
{
    if(event->key() == Qt::Key_Control)
    {
        isCtrlKey = false;
        event->accept();
    }
}


void AbstractGraphicScene::mouseMoveEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
    if ((mouseEvent->scenePos() - dragStartPosition).manhattanLength() > QApplication::startDragDistance() && drawSelect)
    {
        displayRectSelection = true;
    }
    else
    {
        displayRectSelection = false;
    }

    if(displayRectSelection)
    {
        rectSelection.show();
        rectSelection.setRect(posOrigin.x(), posOrigin.y(), mouseEvent->scenePos().x() - posOrigin.x(), mouseEvent->scenePos().y() - posOrigin.y());
    }
    update();
    QGraphicsScene::mouseMoveEvent(mouseEvent);
}


void AbstractGraphicScene::mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
    QGraphicsItem *item = itemAt(mouseEvent->scenePos(), QTransform());

    //item selection
    if(mouseEvent->button() == Qt::LeftButton)
    {
        dragStartPosition = mouseEvent->scenePos();
        if(!item)
        {
            posOrigin = mouseEvent->scenePos();
            drawSelect = true;
        }
    }
    update();
    QGraphicsScene::mousePressEvent(mouseEvent);
    emit clicked();
}


void AbstractGraphicScene::mouseReleaseEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
    if(displayRectSelection)
    {
        if(rectSelection.isVisible())
        {
            UnselectAll();

            for(auto &item : items())
            {

                if(rectSelection.collidesWithItem(item)  && item->isWidget())
                {
                    selectItem(toProxy(item));
                }
            }
        }
    }
    else
    {
        QGraphicsItem *item = itemAt(mouseEvent->scenePos(), QTransform());
        if(!isCtrlKey)
        {
            UnselectAll();
        }
        else if(item)
        {
            QGraphicsProxyWidget* proxy = toProxy(item);
            if(item->isSelected())
            {
                unSelectItem(proxy);
            }
            else
            {
                selectItem(proxy);
            }
        }
    }

    rectSelection.hide();
    drawSelect = false;
    update();
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}
