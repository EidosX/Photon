#include "mainimg.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QPaintEvent>
#include <QPainterPath>

MainImg::MainImg(QWidget *parent)
    : QWidget{parent}
{
    auto* l = new QHBoxLayout(this);
    _noImageSelectedLabel = new QLabel("No image selected");
    auto font = QFont(_noImageSelectedLabel->font());
    font.setPointSize(48);
    _noImageSelectedLabel->setFont(font);
    _noImageSelectedLabel->setStyleSheet("color: #82848A;");
    _noImageSelectedLabel->setAlignment(Qt::AlignCenter);
    l->addWidget(_noImageSelectedLabel);
}

void MainImg::setPath(QString path, std::optional<QRect> crop) {
    _crop = crop;
    _path = path;
    _noImageSelectedLabel->setVisible(false);
    if (path.isEmpty()) _img = {};
    else {
        _img = QPixmap(path);
        if (crop.has_value()) _img = _img->copy(*crop);
    }
    repaint();
}

void MainImg::paintEvent(QPaintEvent *e) {
    if (!_img.has_value()) { _noImageSelectedLabel->setVisible(true); return; }

    int maxW = e->rect().width(),
        maxH = e->rect().height();

    double scaleRatio = std::min(maxW / (double) _img->width(), maxH / (double) _img->height());

    _finalW = _img->width() * scaleRatio,
    _finalH = _img->height() * scaleRatio;

    _offsetX = (maxW - _finalW) / 2,
    _offsetY = (maxH - _finalH) / 2;

    auto _rect = QRect(_offsetX, _offsetY, _finalW, _finalH);

    auto painter = QPainter(this);
    auto clipPath = QPainterPath();
    clipPath.addRoundedRect(_rect, 8, 8);

    painter.setClipPath(clipPath);
    painter.drawPixmap(_rect, _img.value());

    if (_editingCrop && _crop.has_value()) {
        auto cropScreenRect = QRect(_crop->x() / (double) _img->width() * _finalW + _offsetX,
                                    _crop->y()  / (double) _img->height() * _finalH + _offsetY,
                                    _crop->width() * scaleRatio, _crop->height() * scaleRatio);
        auto cropPath = QPainterPath();
        cropPath.addRoundedRect(cropScreenRect, 8, 8);

        painter.setClipPath(clipPath.subtracted(cropPath));
        painter.fillRect(e->rect(), QBrush(QColor(255, 255, 255, 128)));

        painter.setClipping(false);
        painter.strokePath(cropPath, QPen(QColor(0, 0, 0, 64)));
    }
}

void MainImg::mousePressEvent(QMouseEvent* e) {
    _cropSelectInitPos = e->pos();
}

void MainImg::mouseMoveEvent(QMouseEvent* e) {
    if (!_editingCrop) return;

    _crop = QRect();
    _crop->setX((_cropSelectInitPos.x() - _offsetX) / (double) _finalW * _img->width());
    _crop->setY((_cropSelectInitPos.y() - _offsetY) / (double) _finalH * _img->height());
    _crop->setWidth((e->pos().x() - _cropSelectInitPos.x()) / (double) _finalW * _img->width());
    _crop->setHeight((e->pos().y() - _cropSelectInitPos.y()) / (double) _finalH * _img->height());

    _crop = _crop->normalized();

    _crop->setX(std::clamp(_crop->x(), 0, _img->width()));
    _crop->setY(std::clamp(_crop->y(), 0, _img->height()));
    _crop->setWidth(std::clamp(_crop->width(), 0, _img->width() - _crop->x()));
    _crop->setHeight(std::clamp(_crop->height(), 0, _img->height() - _crop->y()));

    repaint();
}

void MainImg::mouseReleaseEvent(QMouseEvent* e) {
    emit editingCropDone();
}
