#ifndef SCENE_H
#define SCENE_H

#include <QWidget>

namespace Ui {
class Scene;
}

class Scene : public QWidget
{
    Q_OBJECT

public:
    explicit Scene(QWidget *parent = nullptr);
    ~Scene();

private:
    Ui::Scene *ui;
};

#endif // SCENE_H
