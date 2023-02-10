#ifndef PREVIEWIMG_H
#define PREVIEWIMG_H

#include <QWidget>

// This is the thumbnail image widget you see in the carousel at the bottom of the app.
// Its peculiarity is that its width adjusts to the aspect ratio of the image, in order
// to correspond to a fixed height.
class PreviewImg : public QWidget
{
    Q_OBJECT
public:
    inline static int HEIGHT = 100;
    explicit PreviewImg(QString path, QWidget *parent = nullptr, std::optional<QRect> crop = {});

    inline void setSelected(bool selected) { _selected = selected; repaint(); }
    inline QString getPath() { return _imagePath; }
    inline std::optional<QRect> getCrop() { return _crop; }
    inline bool isSelected() { return _selected; }

signals:
    void clicked();

protected:
    QSize sizeHint() const override;
    void paintEvent(QPaintEvent*) override;
    inline void mousePressEvent(QMouseEvent*) override { emit clicked(); }

private:
    QString _imagePath;
    std::optional<QRect> _crop;
    QPixmap _img;
    int _width, _height = HEIGHT;
    bool _selected = false;
};

#endif // PREVIEWIMG_H
