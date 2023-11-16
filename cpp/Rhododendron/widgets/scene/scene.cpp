#include "scene.h"
#include "ui_scene.h"

Scene::Scene(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Scene)
{
    ui->setupUi(this);
}

Scene::~Scene()
{
    delete ui;
}
