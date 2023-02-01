#ifndef PREVIEWIMG_H
#define PREVIEWIMG_H

#include <QWidget>

class PreviewImg : public QWidget
{
    Q_OBJECT
public:
    explicit PreviewImg(QString path, QWidget *parent = nullptr);

protected:
    QSize sizeHint() const override;
    void paintEvent(QPaintEvent* e) override;

signals:

private:
    QPixmap _img;
    int _width, _height = 100;
};

#endif // PREVIEWIMG_H
