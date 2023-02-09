#ifndef MAINIMG_H
#define MAINIMG_H

#include <QWidget>
#include <QLabel>

class MainImg : public QWidget
{
    Q_OBJECT
public:
    explicit MainImg(QWidget *parent = nullptr);

    void setPath(QString path);

protected:
    void paintEvent(QPaintEvent* e) override;
signals:

private:
    std::optional<QPixmap> _img = {};
    QString _path;
    QLabel* _noImageSelectedLabel;
};

#endif // MAINIMG_H
