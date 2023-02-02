#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>

struct TagFilter { QString tag; };
struct RatingFilter { int min = 0; };
using Filter = std::variant<TagFilter, RatingFilter>;

struct Image {
    inline Image(QString path) : path(path) {}

    QString path;
    std::vector<QString> tags;
    int rating = 0;
};

class Database : public QObject {
    Q_OBJECT
public:
    // Returns all the images in the database that match the given filters
    virtual std::vector<Image> query(const std::vector<Filter>& filters) = 0;
    virtual Image queryByPath(QString path) = 0;

public slots:
    virtual void addImage(const Image& img) = 0;
    virtual void removeImage(const QString& path) = 0;
    virtual void setRating(QString path, int rating) = 0;
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

private:
    inline bool matchesFilter(const Image& img, const Filter& f) {
        // On verifie que l'image contient bien le tag
        try {
            QString tag = std::get<TagFilter>(f).tag;
            if (std::find(img.tags.begin(), img.tags.end(), tag) == img.tags.end()) return false;
        } catch (const std::bad_variant_access& ex) {}

        // On verifie que l'image a un rating suffisamment elevé
        try {
            int minRating = std::get<RatingFilter>(f).min;
            if (minRating > img.rating) return false;
        } catch (const std::bad_variant_access& ex) {}

        return true;
    }

    std::vector<Image> _images;
};


#endif // DATABASE_H
