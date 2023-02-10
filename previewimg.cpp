#include "previewimg.h"
#include <QPaintEvent>
#include <QPainter>
#include <QPainterPath>

PreviewImg::PreviewImg(QString path, QWidget *parent, std::optional<QRect> crop)
    : QWidget{parent}, _imagePath(std::move(path)), _crop(crop), _img(_imagePath)
{
    if (_crop.has_value()) _img = _img.copy(*_crop);
    double ratio = _height / (double) _img.height();
    _width = (int) (_img.width() * ratio);
    setMinimumSize(_width, _height);
    setCursor(Qt::PointingHandCursor);
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
