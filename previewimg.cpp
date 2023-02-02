#include "previewimg.h"
#include <QPaintEvent>
#include <QPainter>
#include <QPainterPath>

PreviewImg::PreviewImg(QString path, AppState& appState, QWidget *parent)
    : QWidget{parent}, _imagePath(std::move(path)), _img(_imagePath), _appState(appState)
{
    double ratio = _height / (double) _img.height();
    _width = (int) (_img.width() * ratio);
    setMinimumSize(_width, _height);
    setCursor(Qt::PointingHandCursor);
    connect(&appState, &AppState::onSelectedImageChanged, this, &PreviewImg::reloadSelected);
}

QSize PreviewImg::sizeHint() const {
    return {_width, _height};
}

void PreviewImg::paintEvent(QPaintEvent* e) {
    (void) e;
    auto painter = QPainter(this);
    QPainterPath clipPath;
    clipPath.addRoundedRect(0, 0, _width, _height, 8, 8);

    painter.setClipPath(clipPath);
    painter.drawPixmap(0, 0, _width, _height, _img);

    if (_selected) {
        painter.setPen(QPen(QColor(0x0000C2FF), 4));
        painter.drawPath(clipPath);
    }
}

void PreviewImg::mousePressEvent(QMouseEvent*) {
    if (!_selected) _appState.setSelectedImage(_imagePath);
    else _appState.setSelectedImage({});
}

void PreviewImg::reloadSelected() {
    setSelected(_appState.getSelectedImage().has_value() && _imagePath == _appState.getSelectedImage()->path);
}
