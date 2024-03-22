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
    void on_FxButton_toggled(bool checked);

    void on_SceneButton_toggled(bool checked);

private:
    Ui::MainWindow *ui;
    Scene *scene;

    Engine engine;
};
#endif // MAINWINDOW_H
