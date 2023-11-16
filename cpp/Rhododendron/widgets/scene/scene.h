#ifndef SCENE_H
#define SCENE_H

#include <QWidget>
#include <QGraphicsProxyWidget>
#include <QGraphicsScene>
#include <engine/engine.h>
#include <widgets/scene/lampwidget.h>
#include <QGraphicsSceneDragDropEvent>


class GraphicScene : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit GraphicScene(QWidget *parent = nullptr);
    ~GraphicScene();
    void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
    void dropEvent(QGraphicsSceneDragDropEvent *event);
    void dragMoveEvent(QGraphicsSceneDragDropEvent *event);

private:
    bool dragOver;
};


namespace Ui {
class Scene;
}
class Scene : public QWidget
{
    Q_OBJECT

public:
    explicit Scene(QWidget *parent = nullptr, Engine *engine = nullptr);
    ~Scene();
        QGraphicsRectItem  test;

private:
    Ui::Scene *ui;
    GraphicScene *scene;
};

#endif // SCENE_H
