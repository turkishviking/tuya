#include "graphicscene.h"

GraphicScene::GraphicScene(QWidget *parent, Engine *engine, std::map<QString, LampWidget*> *lampWidgets)
{
    engine = engine;
    this->lampWidgets = lampWidgets;
    this->graphicsView = static_cast<QGraphicsView*>(parent);
}


GraphicScene::~GraphicScene()
{

}

void GraphicScene::getSelection()
{


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
    qDebug() << "count" << sceneItems.count(id);
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
