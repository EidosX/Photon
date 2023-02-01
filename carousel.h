#ifndef CAROUSEL_H
#define CAROUSEL_H

#include <QScrollArea>

class Carousel : public QScrollArea
{
    Q_OBJECT
public:
    explicit Carousel(QWidget *parent = nullptr);

protected:
    void wheelEvent(QWheelEvent*) override;
signals:

};

#endif // CAROUSEL_H
