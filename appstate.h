#ifndef APPSTATE_H
#define APPSTATE_H

#include <QDebug>
#include <QObject>
#include "db.h"

class AppState : public QObject
{
    Q_OBJECT
public:
    AppState(std::unique_ptr<Database> db, QObject *parent = nullptr);

    inline const std::optional<Image>& getSelectedImage() { return _selectedImage; }
    inline const std::vector<Image>& getFilteredImages() { return _filteredImages; }

    inline std::unique_ptr<Database>& getDatabase() { return _db; }

signals:
    void onFilteredImagePathsChanged();
    void onSelectedImageChanged();

public slots:
    inline void setSelectedImage(QString path) {
        if (path.isEmpty()) _selectedImage = {};
        else _selectedImage = _db->queryByPath(path);
        updateSelectedImage();
    }
    inline void addImage(const Image& img) { _db->addImage(img); updateFilteredImages(); }
    inline void removeImage(const QString& path) { setSelectedImage(""); _db->removeImage(path); updateFilteredImages(); }

    inline void setSelectedImageRating(int rating) {
        _db->setRating(_selectedImage->path, rating);
        updateFilteredImages();
        updateSelectedImage();
    }

    inline void addTag(QString path, QString tag) {
        _db->addTag(path, tag);
        updateFilteredImages();
        updateSelectedImage();
    }
    inline void removeTag(QString path, QString tag) {
        _db->removeTag(path, tag);
        updateFilteredImages();
        updateSelectedImage();
    }

    inline void setDescription(QString path, QString description) {
        _db->setDescription(path, description);
        updateFilteredImages();
        updateSelectedImage();
    }

    inline void addFilter(const Filter& f) { _filters.push_back(f); updateFilteredImages(); }
    inline void removeFilter(const Filter& f) {
        _filters.erase(std::remove_if(_filters.begin(), _filters.end(), [&](Filter& f1){return filterEquals(f, f1);}));
        updateFilteredImages();
    }

private:
    inline void updateFilteredImages() {
        _filteredImages = _db->query(_filters);
        emit onFilteredImagePathsChanged();
    }
    inline void updateSelectedImage() {
        if (_selectedImage.has_value()) _selectedImage = _db->queryByPath(_selectedImage->path);
        emit onSelectedImageChanged();
    }

    std::vector<Filter> _filters;
    std::unique_ptr<Database> _db;
    std::vector<Image> _filteredImages;
    std::optional<Image> _selectedImage;
};

#endif // APPSTATE_H
