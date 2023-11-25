#include "lampwidget.h"

LampWidget::LampWidget(QWidget *parent, Lamp *l)
    : QToolButton{parent}
{
    this->setText(l->name);
    this->setMinimumSize(QSize(48,48));
    this->setVisible(true);
    this->setCursor(Qt::ClosedHandCursor);
    this->setCheckable(false);
    lamp = *l;
}


void LampWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        dragStartPosition = event->pos();
    }
}


void LampWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (!(event->buttons() & Qt::LeftButton))
        return;
    if ((event->pos() - dragStartPosition).manhattanLength() < QApplication::startDragDistance())
        return;

    QDrag *drag = new QDrag(this);
    QMimeData *mimeData = new QMimeData;

    mimeData->setData("LampWidget", lamp.id.toUtf8());
    mimeData->setData("id", lamp.id.toUtf8());
    mimeData->setData("name", lamp.name.toUtf8());
    drag->setMimeData(mimeData);

    QPixmap widgetPixmap(size());

    render(&widgetPixmap);
    drag->setPixmap(widgetPixmap);
    drag->setHotSpot(QPoint(drag->pixmap().width()/2, drag->pixmap().height()/2));


    Qt::DropAction dropAction = drag->exec(Qt::CopyAction | Qt::MoveAction);

}

void LampWidget::enterEvent(QEnterEvent *event)
{
    //qDebug() << "enterEvent";
    setStyleSheet("\
                    QToolButton\
                    {\
                        color: #f094ef;\
                        border: 4px solid #f096ef;\
                    }\
                    ");
                }

void LampWidget::leaveEvent(QEvent *event)
{
    //qDebug() << "leaveEvent";
    setStyleSheet("\
                    QToolButton \
                    {\
                        background-color: transparent;\
                        color: #f094ef;\
                        border: 4px solid #414141;\
                    }\
                    QToolButton:hover\
                    {\
                      background-color: transparent;\
                      color: #f094ef;\
                      border: 4px solid #414141;\
                    }\
                ");
}

void LampWidget::unSelect()
{
    leaveEvent(nullptr);
}

void LampWidget::select()
{
    enterEvent(nullptr);
}

QString LampWidget::getId()
{
    return lamp.id;
}
