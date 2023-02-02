#include "carousel.h"
#include <QWheelEvent>
#include <QCoreApplication>

Carousel::Carousel(QWidget *parent)
    : QScrollArea{parent}
{
}

QPoint rotate(const QPoint& p) { return QPoint(p.y(), p.x()); }

void Carousel::wheelEvent(QWheelEvent *event) {
    auto* newEvent = new QWheelEvent(event->position(), event->globalPosition(),
                                  rotate(event->pixelDelta()), rotate(event->angleDelta()),
                                  event->buttons(), event->modifiers(),
                                  event->phase(), event->inverted(),
                                  event->source());

    QCoreApplication::sendEvent((QObject*) horizontalScrollBar(), newEvent);
}
