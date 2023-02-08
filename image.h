#ifndef IMAGE_H
#define IMAGE_H

#include <QObject>

struct Image {
    inline Image(QString path) : path(path) {}

    QString path;
    QString description;
    std::vector<QString> tags;
    int rating = 0;
};


#endif // IMAGE_H
