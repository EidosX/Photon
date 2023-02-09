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
    l->addStretch();
    l->addWidget(_noImageSelectedLabel);
    l->addStretch();
}

void MainImg::setPath(QString path) {
    if (path == _path) return;
    _noImageSelectedLabel->setVisible(false);
    _path = path;
    _img = path.isEmpty() ? std::nullopt : std::optional(QPixmap(path));
    repaint();
}

void MainImg::paintEvent(QPaintEvent *e) {
    if (!_img.has_value()) { _noImageSelectedLabel->setVisible(true); return; }

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
