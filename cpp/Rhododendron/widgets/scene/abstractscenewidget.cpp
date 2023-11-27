#include "abstractscenewidget.h"

AbstractSceneWidget::AbstractSceneWidget(QWidget *parent)
{
    setStyleSheet("\
                    background-color: transparent;\
                    color: #f094ef;\
                    border: 4px solid #414141;\
                ");
}

void AbstractSceneWidget::setDatas(QString key, QString value)
{
    mimeDatas.insert(std::make_pair(key, value));
}


void AbstractSceneWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        dragStartPosition = event->pos();
    }
}


void AbstractSceneWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (!(event->buttons() & Qt::LeftButton))
        return;
    if ((event->pos() - dragStartPosition).manhattanLength() < QApplication::startDragDistance())
        return;

    QDrag *drag = new QDrag(this);
    QMimeData *data = new QMimeData;
    for(auto &d : mimeDatas)
    {
        data->setData(d.first, d.second.toUtf8());
    }

    drag->setMimeData(data);

    QPixmap widgetPixmap(size());

    render(&widgetPixmap);
    drag->setPixmap(widgetPixmap);
    drag->setHotSpot(QPoint(drag->pixmap().width()/2, drag->pixmap().height()/2));


    Qt::DropAction dropAction = drag->exec(Qt::CopyAction | Qt::MoveAction);

}

void AbstractSceneWidget::enterEvent(QEnterEvent *event)
{
    //qDebug() << "enterEvent";
    setStyleSheet("\
                    *\
                    {\
                        color: #f094ef;\
                        border: 4px solid #f096ef;\
                    }\
                    ");
                }

void AbstractSceneWidget::leaveEvent(QEvent *event)
{
    //qDebug() << "leaveEvent";
    setStyleSheet("\
                     *\
                    {\
                        background-color: transparent;\
                        color: #f094ef;\
                        border: 4px solid #414141;\
                    }\
                    *:hover\
                    {\
                      background-color: transparent;\
                      color: #f094ef;\
                      border: 4px solid #414141;\
                    }\
                ");
}

void AbstractSceneWidget::unSelect()
{
    leaveEvent(nullptr);
}

void AbstractSceneWidget::select()
{
    enterEvent(nullptr);
}
