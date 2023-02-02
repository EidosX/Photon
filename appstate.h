#ifndef APPSTATE_H
#define APPSTATE_H

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
    inline void setSelectedImage(std::optional<Image> img) {
        _selectedImage = std::move(img);
        emit onSelectedImageChanged();
    }

private:
    inline void setFilteredImages(std::vector<Image> images) {
        _filteredImages = std::move(images);
        emit onFilteredImagePathsChanged();
    }

    std::unique_ptr<Database> _db;
    std::vector<Image> _filteredImages;
    std::optional<Image> _selectedImage;
};

#endif // APPSTATE_H
