#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene = new Scene(nullptr, &engine);
    ui->centralLayout->addWidget(scene);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete scene;
}



void MainWindow::on_SceneButton_clicked()
{
    scene->setVisible(false);
}

