#ifndef SCENE_H
#define SCENE_H

#include <QWidget>
#include <QGraphicsProxyWidget>
#include <QGraphicsScene>
#include <engine/engine.h>
#include <widgets/scene/lampwidget.h>
#include <QGraphicsSceneDragDropEvent>
#include <map>
#include <widgets/graphicsviewzoom.h>

class GraphicScene : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit GraphicScene(QWidget *parent = nullptr, Engine *engine = nullptr, std::map<QString, LampWidget*> *lampWidgets = nullptr);
    ~GraphicScene();
    void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
    void dropEvent(QGraphicsSceneDragDropEvent *event);
    void dragMoveEvent(QGraphicsSceneDragDropEvent *event);
    Engine* engine;

private:
    bool dragOver;
    std::map<QString, LampWidget*> *lampWidgets;
    std::map<QString, QGraphicsProxyWidget*> sceneItems;
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
    Engine* engine;

private:
    Ui::Scene *ui;
    GraphicScene *scene;
    std::map<QString, LampWidget*> lampWidgets;
    GraphicsViewZoom *graphicView;
};

#endif // SCENE_H
