#include "mainimg.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QPaintEvent>
#include <QPainterPath>

MainImg::MainImg(AppState& appState, QWidget *parent)
    : QWidget{parent}, _appState(appState)
{
    connect(&appState, &AppState::onSelectedImageChanged, this, [&](){
        if (_appState.getSelectedImage().has_value())
            setPath(_appState.getSelectedImage()->path);
        else setPath("");
    });
}

void MainImg::setPath(QString path) {
    if (path == _path) return;
    _path = path;
    _img = path.isEmpty() ? std::nullopt : std::optional(QPixmap(path));
    repaint();
}

void MainImg::paintEvent(QPaintEvent *e) {
    if (!_img.has_value()) return;

    int imgW = _img->width(),
        imgH = _img->height();

    int maxW = e->rect().width(),
        maxH = e->rect().height();

    double scaleRatio = std::min(maxW / (double) imgW, maxH / (double) imgH);

    int finalW = imgW * scaleRatio,
        finalH = imgH * scaleRatio;

    int offsetX = (maxW - finalW) / 2,
        offsetY = (maxH - finalH) / 2;

    auto painter = QPainter(this);
    QPainterPath clipPath;
    clipPath.addRoundedRect(offsetX, offsetY, finalW, finalH, 8, 8);

    painter.setClipPath(clipPath);
    painter.drawPixmap(offsetX, offsetY, finalW, finalH, _img.value());
}
