#include "scene.h"
#include "ui_scene.h"

GraphicScene::GraphicScene(QWidget *parent, Engine *engine, std::map<QString, LampWidget*> *lampWidgets)
{
    engine = engine;
    this->lampWidgets = lampWidgets;

    QBrush penBrush(QColor(200,200,200, 150), Qt::BrushStyle::SolidPattern);
    QPen pen(penBrush, 2, Qt::PenStyle::SolidLine);
    rectSelection.setPen(pen);

    QBrush rectBrush(QColor(240,148,239, 50), Qt::BrushStyle::SolidPattern);
    rectSelection.setBrush(rectBrush);
    rectSelection.setZValue(1000);
    addItem(&rectSelection);

    addItem(&groupedSelection);
    groupedSelection.setFlag(QGraphicsItem::ItemIsMovable);
    this->graphicsView = static_cast<QGraphicsView*>(parent);
}


GraphicScene::~GraphicScene()
{
    QToolButton b;
    b.setStyleSheet("");
}

void GraphicScene::getSelection()
{
    selection.clear();
    for(auto &item : proxyItems)
    {
        if(rectSelection.collidesWithItem(item.first))
        {
            item.second->setStyleSheet("border: 4px solid #f094ef;");
            selection.push_back(item.first);
            groupedSelection.addToGroup(item.first);
        }
    }
}

void GraphicScene::mouseMoveEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
    //qDebug() << Q_FUNC_INFO << mouseEvent->scenePos();
    if(drawSelect)
    {
        rectSelection.show();
        rectSelection.setRect(posOrigin.x(), posOrigin.y(), mouseEvent->scenePos().x() - posOrigin.x(), mouseEvent->scenePos().y() - posOrigin.y());
    }
    update();
    QGraphicsScene::mouseMoveEvent(mouseEvent);
}

void GraphicScene::mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
    QGraphicsItem *item = itemAt(mouseEvent->scenePos(), QTransform());
    //qDebug() << Q_FUNC_INFO << mouseEvent->scenePos() << item;

    //item selection
    if(mouseEvent->button() == Qt::LeftButton)
    {
        if(!item)
        {
            posOrigin = mouseEvent->scenePos();
            drawSelect = true;
            for(auto &item:selection)
            {
                proxyItems[item]->setStyleSheet("border: 4px solid #414141;");
            }

            for(auto &item : selection)
            {
                groupedSelection.removeFromGroup(item);
            }

            selection.clear();
        }
    }
    update();
    QGraphicsScene::mousePressEvent(mouseEvent);
}

void GraphicScene::mouseReleaseEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
    //qDebug() << Q_FUNC_INFO << mouseEvent->scenePos();
    if(drawSelect && rectSelection.isVisible())
    {
        getSelection();
    }
    rectSelection.hide();
    drawSelect = false;
    update();
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

void GraphicScene::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{

    if(event->mimeData()->data("LampWidget").size() != 0)
    {
        event->setAccepted(true);
        dragOver = true;
    }
    else
    {
        event->setAccepted(false);
    }
    update();
}

void GraphicScene::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    if(event->mimeData()->data("LampWidget").size() != 0)
    {
        event->setAccepted(true);
    }
    update();
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
        proxyItems.insert(std::make_pair(item, lampWidget));
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

    scene->setSceneRect(-16363,-16363, 32726, 32726);
    scene->update();

}

Scene::~Scene()
{
    delete ui;
}
