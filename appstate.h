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
    inline void setSelectedImage(std::optional<Image> img) { _selectedImage = std::move(img); emit onSelectedImageChanged(); }
    inline void addImage(const Image& img) { _db->addImage(img); updateFilteredImages(); }

private:
    inline void updateFilteredImages() {
        _filteredImages = _db->query(_filters);
        emit onFilteredImagePathsChanged();
    }

    std::vector<Filter> _filters;
    std::unique_ptr<Database> _db;
    std::vector<Image> _filteredImages;
    std::optional<Image> _selectedImage;
};

#endif // APPSTATE_H
