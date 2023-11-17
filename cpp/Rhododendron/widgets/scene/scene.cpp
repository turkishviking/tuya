#include "scene.h"
#include "ui_scene.h"

GraphicScene::GraphicScene(QWidget *parent, Engine *engine, std::map<QString, LampWidget*> *lampWidgets)
{
    engine = engine;
    this->lampWidgets = lampWidgets;
}


GraphicScene::~GraphicScene()
{

}

void GraphicScene::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{

    if(event->mimeData()->data("LampWidget").size() != 0)
    {
        event->setAccepted(true);
        dragOver = true;
        update();
    }
    else
    {
        event->setAccepted(false);
    }
}

void GraphicScene::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    if(event->mimeData()->data("LampWidget").size() != 0)
    {
        event->setAccepted(true);
    }
}

void GraphicScene::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    event->setAccepted(true);
    QString id = event->mimeData()->data("id");

    LampWidget *lampWidget = lampWidgets->at(id);
    lampWidget->setParent(NULL);

    QGraphicsProxyWidget * item;
    if (sceneItems.count(id))
    {
        item = sceneItems[id];
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
    update();
    dragOver = false;

}


Scene::Scene(QWidget *parent,Engine *engine) :
    QWidget(parent),
    ui(new Ui::Scene)
{
    ui->setupUi(this);
    engine = engine;

    for(auto &lamp : engine->getLamps())
    {
        LampWidget *lampWidget = new LampWidget(NULL, &lamp);
        ui->verticalLayout_scene_2->insertWidget(0,lampWidget);
        lampWidgets.insert(std::make_pair(lamp.id, lampWidget));
    }

    graphicView = new GraphicsViewZoom(ui->graphicsView);
    graphicView->set_modifiers(Qt::NoModifier);
    scene = new GraphicScene(ui->graphicsView, engine, &lampWidgets);
    ui->graphicsView->setScene(scene);

    // color
    QBrush brush;
    brush.setColor(QColor(25,25,25,255));
    brush.setStyle(Qt::SolidPattern);
    scene->setBackgroundBrush(brush);

    setAcceptDrops(true);
    ui->graphicsView->setAcceptDrops(true);

    scene->setSceneRect(-100000,-100000, 200000, 200000);
    scene->update();

}

Scene::~Scene()
{
    delete ui;
}
