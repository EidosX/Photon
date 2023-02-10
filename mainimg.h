#ifndef MAINIMG_H
#define MAINIMG_H

#include <QWidget>
#include <QLabel>
#include <image.h>

// This is the main widget that displays the big sized image when you select one.
// It has support for selecting a cropped region
class MainImg : public QWidget
{
    Q_OBJECT
public:
    explicit MainImg(QWidget *parent = nullptr);

    void setPath(QString path, std::optional<QRect> crop = {});

    inline void setEditingCrop(bool editing) { _editingCrop = editing; }
    inline std::optional<QRect> getCrop() { return _crop; }

signals:
    void editingCropDone();

protected:
    void paintEvent(QPaintEvent* e) override;
    void mousePressEvent(QMouseEvent* e) override;
    void mouseMoveEvent(QMouseEvent* e) override;
    void mouseReleaseEvent(QMouseEvent* e) override;
signals:

private:
    std::optional<QPixmap> _img = {};
    QString _path;
    QLabel* _noImageSelectedLabel;

    int _offsetX = 0, _offsetY = 0;
    int _finalW = 0, _finalH = 0;

    QPoint _cropSelectInitPos;
    std::optional<QRect> _crop;
    bool _editingCrop = false;
};

#endif // MAINIMG_H
