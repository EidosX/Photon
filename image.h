#ifndef IMAGE_H
#define IMAGE_H

#include <QObject>
#include <QRect>

struct Image {
    inline Image(QString path) : path(path) {}

    QString path;
    QString description;
    std::vector<QString> tags;
    int rating = 0;
    std::optional<QRect> crop;
};


#endif // IMAGE_H
