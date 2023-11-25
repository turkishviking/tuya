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
#include <set>

class GraphicScene : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit GraphicScene(QWidget *parent = nullptr, Engine *engine = nullptr, std::map<QString, LampWidget*> *lampWidgets = nullptr);
    ~GraphicScene();
    void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
    void dropEvent(QGraphicsSceneDragDropEvent *event);
    void dragMoveEvent(QGraphicsSceneDragDropEvent *event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * mouseEvent);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * mouseEvent);
    virtual void keyPressEvent(QKeyEvent * event);
    virtual void keyReleaseEvent(QKeyEvent * event);

public:
    Engine* engine;

signals:
    void itemRemoved(LampWidget* lamp);

private:
    void getSelection();
    void selectItem(QGraphicsProxyWidget* item);
    void unSelectItem(QGraphicsProxyWidget* item);
    bool isSelected(QGraphicsProxyWidget* item);
    void UnselectAll();
    void removeSelectedFromScene();
private:
    bool dragOver;
    std::map<QString, LampWidget*> *lampWidgets;
    std::map<QString, QGraphicsProxyWidget*> sceneItems;
    std::map<QGraphicsProxyWidget*, LampWidget*> proxyItems;
    QPointF posOrigin;
    bool leftButtonPressed = false;
    bool drawSelect = false;
    bool displayRectSelection = false;
    QPointF dragStartPosition = QPointF(0,0);
    QGraphicsRectItem rectSelection;
    QGraphicsView* graphicsView;
    std::set<QGraphicsProxyWidget*> selection;
    QGraphicsItemGroup groupedSelection;
    bool isCtrlKey = false;
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

public slots:
    void addWidget(LampWidget* lamp);

private:
    Ui::Scene *ui;
    GraphicScene *scene;
    std::map<QString, LampWidget*> lampWidgets;
    GraphicsViewZoom *graphicView;
};

#endif // SCENE_H
