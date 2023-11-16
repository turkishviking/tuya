#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    centralLayout = new QVBoxLayout;
    scene = new Scene;
    centralLayout->addWidget(scene);
    ui->centralBox->setLayout(centralLayout);
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_SceneButton_clicked()
{
    scene->setVisible(false);
}

