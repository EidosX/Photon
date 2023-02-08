#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include "filter.h"
#include "image.h"

class Database : public QObject {
    Q_OBJECT
public:
    // Returns all the images in the database that match the given filters
    virtual std::vector<Image> query(const std::vector<Filter>& filters) = 0;
    // Returns the image with the given path
    virtual Image queryByPath(QString path) = 0;

public slots:
    // Adds a new image
    virtual void addImage(const Image& img) = 0;
    // Removes an image
    virtual void removeImage(const QString& path) = 0;
    // Sets the rating of the image with the given path
    virtual void setRating(QString path, int rating) = 0;
    // Adds a given tag to the image with the given path
    virtual void addTag(QString path, QString tag) = 0;
    // Removes the given tag from the image with the given path
    virtual void removeTag(QString path, QString tag) = 0;
    // Sets description for the image with given path
    virtual void setDescription(QString path, QString description) = 0;
};

class VectorDatabase : public Database {
    Q_OBJECT
public:
    inline std::vector<Image> query(const std::vector<Filter>& filters) override {
        std::vector<Image> l;
        for (const auto& img : _images) {
            if (std::all_of(filters.cbegin(), filters.cend(), [&](const Filter& f){ return matchesFilter(img, f); }))
                l.push_back(img);
        }
        return l;
    }
    inline Image queryByPath(QString path) override { return queryRefByPath(path); }
    inline Image& queryRefByPath(QString path) {
        for (Image& img : _images) if (img.path == path) return img;
        throw std::runtime_error("Path not found");
    }

public slots:
    inline void addImage(const Image& img) override {
        if (std::any_of(_images.begin(), _images.end(), [&](Image& x){return x.path == img.path;})) return;
        _images.push_back(img);
    }
    inline void removeImage(const QString& path) override {
        _images.erase(std::remove_if(_images.begin(), _images.end(), [&](Image& img){return img.path == path;}));
    }
    inline void setRating(QString path, int rating) override { queryRefByPath(path).rating = rating; }

    inline void addTag(QString path, QString tag) override { queryRefByPath(path).tags.push_back(tag); }

    inline void removeTag(QString path, QString tag) override {
        auto& tags = queryRefByPath(path).tags;
        tags.erase(std::remove_if(tags.begin(), tags.end(), [&](QString& t){return t == tag;}));
    }

    inline void setDescription(QString path, QString description) override { queryRefByPath(path).description = description; }

private:
    std::vector<Image> _images;
};


#endif // DATABASE_H
