#include "carousel.h"
#include <QWheelEvent>
#include <QCoreApplication>

Carousel::Carousel(QWidget *parent)
    : QScrollArea{parent}
{

}

void Carousel::wheelEvent(QWheelEvent *event)
{
    auto rotate = [](const QPoint& point) {return QPoint(point.y(), point.x());};

    auto* newEvent = new QWheelEvent(event->position(), event->globalPosition(),
                                  rotate(event->pixelDelta()), rotate(event->angleDelta()),
                                  event->buttons(), event->modifiers(),
                                  event->phase(), event->inverted(),
                                  event->source());

    QCoreApplication::sendEvent((QObject*) horizontalScrollBar(), newEvent);
}
