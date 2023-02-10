#ifndef STAR_H
#define STAR_H

#include <QLabel>

// This is a single star widget, that can be clicked, enabled (gold) or disabled (greyed)
class Star : public QLabel {
    Q_OBJECT
public:
    inline Star(QWidget* parent)
        : QLabel(parent)
    {
        setCursor(Qt::PointingHandCursor);
        setScaledContents(true);
        enable(true);
    }

signals:
    void clicked();

public slots:
    inline void enable(bool enabled) {
        setPixmap(QPixmap(enabled ? ":/res/star.png" : ":/res/star_greyed.png"));
    }

protected:
    inline void mousePressEvent(QMouseEvent*) override { emit clicked(); }
};

#endif // STAR_H
