#ifndef GROUPWIDGET_H
#define GROUPWIDGET_H

#include <QWidget>
#include <QStyleOption>
#include <QPainter>
#include <QGraphicsItemGroup>
#include <QGraphicsProxyWidget>
#include <widgets/scene/lampwidget.h>
#include <set>

namespace Ui {
class GroupWidget;
}

class GroupWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GroupWidget(QWidget *parent = nullptr);
    ~GroupWidget();
    void paintEvent(QPaintEvent* event);
    void updateSelection(QGraphicsItemGroup &selection);

private slots:
    void on_lineEdit_returnPressed();
    void on_pushButton_clicked();

private:
    Ui::GroupWidget *ui;
    std::map<QString, std::set<Lamp*>> groups;
    std::set<Lamp*> selection;

private:
    void updateGroups();
};

#endif // GROUPWIDGET_H
