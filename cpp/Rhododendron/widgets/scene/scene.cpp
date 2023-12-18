#include "scene.h"
#include "ui_scene.h"


Scene::Scene(QWidget *parent,Engine *engine) :
    QWidget(parent),
    ui(new Ui::Scene)
{
    ui->setupUi(this);
    engine = engine;

    engine->groupWidget = new GroupWidget(this->ui->groupBox_scene_2);
    engine->groupWidget->hide();

    lampsLayout.setContentsMargins(QMargins(0,0,0,0));
    ui->buttonWidget->setLayout(&lampsLayout);

    for(auto &lamp : engine->getLamps())
    {
        LampWidget *lampWidget = new LampWidget(NULL, &lamp);
        addWidget(lampWidget);
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

    connect(scene, &AbstractGraphicScene::itemRemoved, this, &Scene::addProxyWidget);
}

void Scene::addProxyWidget(QGraphicsProxyWidget* proxy)
{
    auto* lampWidget = static_cast<LampWidget*>(proxy->widget());
    qDebug() << lampWidget << proxy;
    addWidget(lampWidget);
}

void Scene::addWidget(LampWidget* lampWidget)
{

    lampWidgets.insert(std::make_pair(lampWidget->getName(), lampWidget));
    std::set<QString> list;
    for(auto w: lampsLayout.children())
    {
        list.insert(static_cast<LampWidget*>(w)->getName());
    }

    int k = distance(list.begin(), list.find(lampWidget->getName()));
    qDebug() << lampWidgets.size() << lampWidget->getName();
    //if(lampsLayout.count() < k)
    lampsLayout.insertWidget(k, lampWidget);
}

Scene::~Scene()
{
    delete ui;
}
