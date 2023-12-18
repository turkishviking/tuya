#ifndef GRAPHICSCENE_H
#define GRAPHICSCENE_H

#include <QWidget>
#include <widgets/scene/abstractgraphicscene.h>
#include <engine/engine.h>
#include <widgets/scene/lampwidget.h>
#include <QGraphicsSceneDragDropEvent>
#include <QGraphicsProxyWidget>
#include <QGraphicsView>
#include <set>
#include <map>


class GraphicScene : public AbstractGraphicScene
{
    Q_OBJECT

public:
    explicit GraphicScene(QWidget *parent = nullptr, Engine *engine = nullptr, std::map<QString, LampWidget*> *lampWidgets = nullptr);
    ~GraphicScene();
    void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
    void dropEvent(QGraphicsSceneDragDropEvent *event);
    void dragMoveEvent(QGraphicsSceneDragDropEvent *event);

public:
    Engine* engine;


private:
    void keyReleaseEvent(QKeyEvent * event);
    void setGroupWidgetToCursorPosition();

private:
    bool dragOver;
    std::map<QString, LampWidget*> *lampWidgets;
    std::map<QString, QGraphicsProxyWidget*> sceneItems;
    bool leftButtonPressed = false;
    QGraphicsView* graphicsView;
};

#endif // GRAPHICSCENE_H
