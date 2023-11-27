#ifndef ABSTRACTGRAPHICSCENE_H
#define ABSTRACTGRAPHICSCENE_H

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsProxyWidget>
#include <QGraphicsSceneDragDropEvent>
#include <QApplication>
#include <widgets/scene/abstractscenewidget.h>

class AbstractGraphicScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit AbstractGraphicScene(QObject *parent = nullptr);

private:
    void UnselectAll();
    QGraphicsProxyWidget* toProxy(QGraphicsItem* item);
private:
    QGraphicsRectItem rectSelection;
    QGraphicsItemGroup groupedSelection;
    bool isCtrlKey = false;
    bool displayRectSelection = false;
    bool drawSelect = false;
    QPointF dragStartPosition = QPointF(0,0);
    QPointF posOrigin;

protected:
    void selectItem(QGraphicsProxyWidget* item);
    void unSelectItem(QGraphicsProxyWidget* item);
    void removeSelectedFromScene();
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * mouseEvent);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * mouseEvent);
    virtual void keyPressEvent(QKeyEvent * event);
    virtual void keyReleaseEvent(QKeyEvent * event);



signals:
    void itemRemoved(QGraphicsProxyWidget* item);
};

#endif // ABSTRACTGRAPHICSCENE_H
