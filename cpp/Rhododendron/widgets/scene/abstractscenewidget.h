#ifndef ABSTRACTSCENEWIDGET_H
#define ABSTRACTSCENEWIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include <QPointF>
#include <QApplication>
#include <QDrag>
#include <QMimeData>

class AbstractSceneWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AbstractSceneWidget(QWidget *parent = nullptr);
    void unSelect();
    void select();

private:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void enterEvent(QEnterEvent *event);
    void leaveEvent(QEvent *event);

private:
    QPointF dragStartPosition;
    std::map<QString, QString> mimeDatas;

protected:
    void setDatas(QString key, QString value);
signals:

};

#endif // ABSTRACTSCENEWIDGET_H
