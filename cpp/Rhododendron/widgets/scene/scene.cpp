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


}

bool GraphicScene::isSelected(QGraphicsProxyWidget* item)
{
    for(auto &e:selection)
        qDebug() << e << item;
    return std::find(selection.begin(), selection.end(), item) != selection.end();
}

void GraphicScene::UnselectAll()
{
    if(!selection.empty())
    {
        for (auto it = selection.begin(); it != selection.end(); )
        {
            if (true)
            {
                unSelectItem(*it);
                it = selection.erase(it);
            }
            else {
                ++it;
            }
        }
        selection.clear();
    }
}

void GraphicScene::selectItem(QGraphicsProxyWidget* item)
{
    proxyItems[item]->setStyleSheet("\
                                    QToolButton\
                                    {\
                                        color: #f094ef;\
                                        border: 4px solid #f094ef;\
                                    }\
                                    ");
    selection.insert(item);
    groupedSelection.addToGroup(item);
}

void GraphicScene::unSelectItem(QGraphicsProxyWidget* item)
{
    groupedSelection.removeFromGroup(item);
    proxyItems[item]->setStyleSheet("\
                                        QToolButton \
                                        {\
                                            background-color: transparent;\
                                            color: #f094ef;\
                                            border: 4px solid #414141;\
                                        }\
                                        QToolButton:hover\
                                        {\
                                            background-color: transparent;\
                                            color: #f094ef;\
                                            border: 4px solid #ffba60;\
                                        }\
                                    ");
    update();
}

void GraphicScene::keyPressEvent(QKeyEvent * event)
{
    //qDebug() << event->key();
    if(event->key() == Qt::Key_Control)
    {
        isCtrlKey = true;
        event->accept();
    }
    else if(event->key() == Qt::Key_Delete || event->key() == Qt::Key_Backspace)
    {
        qDebug() << "ddd";
    }
}

void GraphicScene::keyReleaseEvent(QKeyEvent * event)
{
    if(event->key() == Qt::Key_Control)
    {
        isCtrlKey = false;
        event->accept();
    }
}

void GraphicScene::mouseMoveEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
    //qDebug() << "man" << ((mouseEvent->scenePos() - dragStartPosition).manhattanLength() > QApplication::startDragDistance()) << drawSelect;
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

void GraphicScene::mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
    QGraphicsItem *item = itemAt(mouseEvent->scenePos(), QTransform());
    //qDebug() << Q_FUNC_INFO << item;

    //item selection
    if(mouseEvent->button() == Qt::LeftButton)
    {
        leftButtonPressed = true;
        dragStartPosition = mouseEvent->scenePos();
        if(!item)
        {
            posOrigin = mouseEvent->scenePos();
            drawSelect = true;
        }
    }
    update();
    QGraphicsScene::mousePressEvent(mouseEvent);
}

void GraphicScene::mouseReleaseEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
    //qDebug() << drawSelect << displayRectSelection;
    if(displayRectSelection)
    {
        if(rectSelection.isVisible())
        {
            UnselectAll();
            qDebug() << "Unselect select rect";
            for(auto &item : proxyItems)
            {
                if(rectSelection.collidesWithItem(item.first))
                {
                    selectItem(item.first);
                }
            }
        }
    }
    else
    {
        QGraphicsItem *item = itemAt(mouseEvent->scenePos(), QTransform());
        if(!isCtrlKey)
        {
            qDebug() << "Unselect select ctrl";
            UnselectAll();
        }


        else if(item)
        {
            QGraphicsProxyWidget* proxy = static_cast<QGraphicsProxyWidget*>(item);
            qDebug() << "before" << isSelected(proxy);
            if(isSelected(proxy))
            {
                unSelectItem(proxy);
                selection.erase(proxy);
                qDebug() << "unselect" << isSelected(proxy) << isSelected(proxy);
            }
            else
            {
                selectItem(proxy);
            }
            qDebug() << "after" << isSelected(proxy);
        }
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
