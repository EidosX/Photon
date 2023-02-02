#ifndef STAR_H
#define STAR_H

#include "appstate.h"
#include <QLabel>

class Star : public QLabel
{
    Q_OBJECT
public:
    inline Star(AppState& appState, int number, QWidget* parent)
        : QLabel(parent), _number(number), _appState(appState)
    {
        setCursor(Qt::PointingHandCursor);
        setMaximumSize(20, 20);
        setMinimumSize(20, 20);
        setScaledContents(true);
        update();
        connect(&_appState, &AppState::onSelectedImageChanged, this, &Star::update);
    }

    inline void enable(bool enabled) {
        setPixmap(QPixmap(enabled ? ":/res/star.png" : ":/res/star_greyed.png"));
    }

public slots:
    inline void update() {
        int currentStarsCount = _appState.getSelectedImage().has_value()
                ? _appState.getSelectedImage().value().rating : 0;
        enable(_number <= currentStarsCount);
    }

protected:
    inline void mousePressEvent(QMouseEvent*) override {
        if (!_appState.getSelectedImage().has_value()) return;
        _appState.setSelectedImageRating(_number);
    }

private:
    int _number;
    AppState& _appState;
};

#endif // STAR_H
