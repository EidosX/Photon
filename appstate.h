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

signals:
    void onFilteredImagePathsChanged();
    void onSelectedImageChanged();

public slots:
    inline void setSelectedImage(std::optional<Image> img) { _selectedImage = std::move(img); updateSelectedImage(); }
    inline void addImage(const Image& img) { _db->addImage(img); updateFilteredImages(); }

    inline void setSelectedImageRating(int rating) {
        _db->setRating(_selectedImage->path, rating);
        updateFilteredImages();
        updateSelectedImage();
    }

private:
    inline void updateFilteredImages() {
        _filteredImages = _db->query(_filters);
        emit onFilteredImagePathsChanged();
    }
    inline void updateSelectedImage() {
        if (!_selectedImage.has_value()) return;
        _selectedImage = _db->queryByPath(_selectedImage->path);
        emit onSelectedImageChanged();
    }

    std::vector<Filter> _filters;
    std::unique_ptr<Database> _db;
    std::vector<Image> _filteredImages;
    std::optional<Image> _selectedImage;
};

#endif // APPSTATE_H
