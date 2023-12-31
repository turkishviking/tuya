#include "graphicsviewzoom.h"
#include <QMouseEvent>
#include <QApplication>
#include <QScrollBar>
#include <qmath.h>

GraphicsViewZoom::GraphicsViewZoom(QGraphicsView* view)
    : QObject(view), _view(view)
{
    _view->viewport()->installEventFilter(this);
    _view->setMouseTracking(true);
    _modifiers = Qt::ControlModifier;
    _zoom_factor_base = 1.0015;
    // Hide the scrollbars
    _view->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    _view->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
}

void GraphicsViewZoom::gentle_zoom(double factor)
{
    _view->scale(factor, factor);
    _view->centerOn(target_scene_pos);
    QPointF delta_viewport_pos = target_viewport_pos - QPointF(_view->viewport()->width() / 2.0,
                                                               _view->viewport()->height() / 2.0);
    QPointF viewport_center = _view->mapFromScene(target_scene_pos) - delta_viewport_pos;
    _view->centerOn(_view->mapToScene(viewport_center.toPoint()));
    emit zoomed();
}

void GraphicsViewZoom::set_modifiers(Qt::KeyboardModifiers modifiers)
{
    _modifiers = modifiers;

}

void GraphicsViewZoom::set_zoom_factor_base(double value)
{
    _zoom_factor_base = value;
}

bool GraphicsViewZoom::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::MouseMove)
    {
        //Zoom
        QMouseEvent* mouse_event = static_cast<QMouseEvent*>(event);
        QPointF delta = target_viewport_pos - mouse_event->pos();
        if (qAbs(delta.x()) > 5 || qAbs(delta.y()) > 5)
        {
            target_viewport_pos = mouse_event->pos();
            target_scene_pos = _view->mapToScene(mouse_event->pos());
        }


        //Translation
        if (mouse_event->buttons() & Qt::RightButton)
        {
            QPointF oldp = _view->mapToScene(posOrigin);
            QPointF newP = _view->mapToScene(mouse_event->pos());
            QPointF translation = oldp - newP;

            _view->horizontalScrollBar()->setValue( _view->horizontalScrollBar()->value() + translation.x() );
            _view->verticalScrollBar()->setValue( _view->verticalScrollBar()->value() + translation.y());

            posOrigin = mouse_event->pos();
        }
    }
    else if (event->type() == QEvent::Wheel)
    {
        QWheelEvent* wheel_event = static_cast<QWheelEvent*>(event);
        if (QApplication::keyboardModifiers() == _modifiers)
        {
            double angle = wheel_event->angleDelta().y();
            double factor = qPow(_zoom_factor_base, angle);
            gentle_zoom(factor);
            return true;
        }
    }
    else if (event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        if (mouseEvent->button() == Qt::RightButton)
        {
            // Store original position.
            posOrigin = mouseEvent->pos();
        }
    }

    Q_UNUSED(object)
    return false;
}
