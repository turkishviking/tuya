#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <widgets/scene/scene.h>
#include <engine/engine.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_SceneButton_clicked();

private:
    Ui::MainWindow *ui;
    QVBoxLayout *centralLayout;
    Scene *scene;

    Engine engine;
};
#endif // MAINWINDOW_H
