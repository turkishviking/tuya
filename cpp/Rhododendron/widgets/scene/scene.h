#ifndef SCENE_H
#define SCENE_H

#include "qboxlayout.h"
#include <QWidget>
#include <engine/engine.h>
#include <map>
#include <widgets/graphicsviewzoom.h>
#include <widgets/scene/graphicscene.h>
#include "widgets/scene/groupwidget.h"


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
    void addProxyWidget(QGraphicsProxyWidget* proxy);

private:
    Ui::Scene *ui;
    GraphicScene *scene;
    std::map<QString, LampWidget*> lampWidgets;
    GraphicsViewZoom *graphicView;
    QVBoxLayout lampsLayout;

private:
    void addWidget(LampWidget* lampWidget);

};

#endif // SCENE_H
