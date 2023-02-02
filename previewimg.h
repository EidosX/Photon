#ifndef PREVIEWIMG_H
#define PREVIEWIMG_H

#include <QWidget>
#include "db.h"
#include "appstate.h"

class PreviewImg : public QWidget
{
    Q_OBJECT
public:
    inline static int HEIGHT = 100;
    explicit PreviewImg(QString path, AppState& appState, QWidget *parent = nullptr);

    inline void setSelected(bool selected) { _selected = selected; repaint(); }
    inline Image getImage() { return _appState.getDatabase()->queryByPath(_imagePath); }
    inline QString getPath() { return _imagePath; }

protected:
    QSize sizeHint() const override;
    void paintEvent(QPaintEvent*) override;
    void mousePressEvent(QMouseEvent*) override;

public slots:
    void reloadSelected();

private:
    QString _imagePath;
    QPixmap _img;
    int _width, _height = HEIGHT;
    bool _selected = false;
    AppState& _appState;
};

#endif // PREVIEWIMG_H
