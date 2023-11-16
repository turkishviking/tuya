#include "scene.h"
#include "ui_scene.h"

Scene::Scene(QWidget *parent,Engine *engine) :
    QWidget(parent),
    ui(new Ui::Scene)
{
    ui->setupUi(this);
    scene = new GraphicScene((ui->graphicsView));
    ui->graphicsView->setScene(scene);

    // color
    QBrush brush;
    brush.setColor(QColor(25,25,25,255));
    brush.setStyle(Qt::SolidPattern);
    scene->setBackgroundBrush(brush);

    setAcceptDrops(true);
    ui->graphicsView->setAcceptDrops(true);

    for(auto &lamp : engine->getLamps())
    {
        LampWidget *lampWidget = new LampWidget(this, &lamp);
        ui->verticalLayout_scene_2->insertWidget(0,lampWidget);
    }

    scene->setSceneRect(QRect(0,0,1000,1000));


    test.setRect(100, 100, 200, 200);
    brush.setColor(QColor(255,255,255,255));
    test.setBrush(brush);
    scene->addItem(&test);

    test.setPos(QPointF(200,200));
    test.setZValue(1);

    scene->update();
    qDebug() << scene->sceneRect().center();
    ui->graphicsView->fitInView(scene->sceneRect());

}

Scene::~Scene()
{
    delete ui;
    delete scene;
}


GraphicScene::GraphicScene(QWidget *parent)
{

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
    qDebug() << event->mimeData()->data("name");
    Lamp lamp;
    event->source();
    lamp.id = event->mimeData()->data("id");
    lamp.name = event->mimeData()->data("name");
    LampWidget widget(nullptr, &lamp);
    QGraphicsProxyWidget * item = addWidget(event->source().);
    //item->setParentItem(anOtherItem);


    item->setPos(QPointF(200,200));
    item->setZValue(1);
    update();
    /*
    dragOver = false;
    if (event->mimeData()->hasColor())
        color = qvariant_cast<QColor>(event->mimeData()->colorData());
    update();*/
}
