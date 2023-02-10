#ifndef CAROUSEL_H
#define CAROUSEL_H

#include <QScrollArea>

// The carousel is the horizontal scrolling list of images
// at the bottom of the application.
class Carousel : public QScrollArea {
    Q_OBJECT
public:
    explicit Carousel(QWidget *parent = nullptr);

protected:
    void wheelEvent(QWheelEvent*) override;
signals:

};

#endif // CAROUSEL_H
