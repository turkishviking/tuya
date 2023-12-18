#include "graphicscene.h"

GraphicScene::GraphicScene(QWidget *parent, Engine *engine, std::map<QString, LampWidget*> *lampWidgets)
{
    this->engine = engine;
    this->lampWidgets = lampWidgets;
    this->graphicsView = static_cast<QGraphicsView*>(parent);

    connect(this, &AbstractGraphicScene::clicked, engine->groupWidget, &GroupWidget::hide);

}


GraphicScene::~GraphicScene()
{

}

void GraphicScene::setGroupWidgetToCursorPosition()
{
    // prevent groupWidget to go out off screen
    QPoint pos;
    QPoint groupPos = graphicsView->mapFromGlobal(QCursor::pos());
    QSize groupSize = engine->groupWidget->size();
    //center
    groupPos.setX(groupPos.x() - groupSize.width()/2);
    groupPos.setY(groupPos.y() - groupSize.height()/2);

    QPoint viewPos = graphicsView->pos();
    QSize viewSize = graphicsView->size();


    if(groupPos.x() + groupSize.width() >= viewPos.x() + viewSize.width())
        pos.setX(viewPos.x() + viewSize.width() - groupSize.width());
    else if(groupPos.x() <= viewPos.x())
        pos.setX(viewPos.x());
    else
        pos.setX(groupPos.x());

    if(groupPos.y() + groupSize.height() >= viewPos.y() + viewSize.height())
        pos.setY(viewPos.y() + viewSize.height() - groupSize.height());
    else if(groupPos.y() <= viewPos.y())
        pos.setY(viewPos.y());
    else
        pos.setY(groupPos.y());

    engine->groupWidget->move(pos);
}

void GraphicScene::keyReleaseEvent(QKeyEvent * event)
{
    if(event->key() == Qt::Key_G)
    {

        if(engine->groupWidget->isVisible())
        {
            engine->groupWidget->hide();
        }
        else
        {
            setGroupWidgetToCursorPosition();
            engine->groupWidget->updateSelection(groupedSelection);
            engine->groupWidget->show();
        }

        event->accept();
    }
}


void GraphicScene::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{

    if(event->mimeData()->data("LampWidget").size() != 0)
    {
        //event->setAccepted(true);
        dragOver = true;
    }
    else
    {
        //event->setAccepted(false);
    }
    update();
}

void GraphicScene::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    if(event->mimeData()->data("LampWidget").size() != 0)
    {
        //event->setAccepted(true);
    }
    update();
}

void GraphicScene::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    //event->setAccepted(true);
    QString id = event->mimeData()->data("id");
    QString name = event->mimeData()->data("name");

    LampWidget *lampWidget = lampWidgets->at(name);
    lampWidget->setParent(NULL);

    QGraphicsProxyWidget * item;
    //qDebug() << "count" << sceneItems.count(id);
    if (sceneItems.count(id))
    {
        if(sceneItems[id]->widget())
        {
            // existing element
            item = sceneItems[id];
        }
        else
        {
            // remove old proxy, redrop a new one with same lampWidget
            sceneItems.erase(id);
            item = addWidget(lampWidget);
            sceneItems.insert(std::make_pair(id, item));
        }
    }
    else
    {
        item = addWidget(lampWidget);
        sceneItems.insert(std::make_pair(id, item));
    }

    //item->setParentItem(anOtherItem);
    QPointF pos;
    pos.setX(event->scenePos().rx() - item->boundingRect().width()/2);
    pos.setY(event->scenePos().ry() - item->boundingRect().height()/2);
    item->setPos(pos);
    item->setZValue(1);

    dragOver = false;
    update();
}
