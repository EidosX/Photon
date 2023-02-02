#ifndef MAINIMG_H
#define MAINIMG_H

#include <QWidget>
#include "appstate.h"

class MainImg : public QWidget
{
    Q_OBJECT
public:
    explicit MainImg(AppState& appState, QWidget *parent = nullptr);

    void setPath(QString path);

protected:
    void paintEvent(QPaintEvent* e) override;
signals:

private:
    std::optional<QPixmap> _img = {};
    AppState& _appState;
};

#endif // MAINIMG_H
