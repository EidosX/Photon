#include "appstate.h"


AppState::AppState(std::unique_ptr<Database> db, QObject *parent)
    : QObject{parent}, _db(std::move(db))
{
    _filteredImages = _db->query({});
}


const std::optional<Image>& AppState::getSelectedImage() {
    return _selectedImage;
}

const std::vector<Image>& AppState::getFilteredImages() {
    return _filteredImages;
}

const std::vector<Filter>& AppState::getFilters() {
    return _filters;
}

void AppState::setSelectedImage(QString path) {
    if (path.isEmpty()) _selectedImage = {};
    else _selectedImage = _db->queryByPath(path);
    updateSelectedImage();
}

void AppState::addImage(const Image& img) {
    _db->addImage(img); updateFilteredImages();
}

void AppState::removeImage(const QString& path) {
    _db->removeImage(path); setSelectedImage(""); updateFilteredImages();
}

void AppState::setSelectedImageRating(int rating) {
    _db->setRating(_selectedImage->path, rating);
    updateSelectedImage();
    updateFilteredImages();
}

void AppState::addTag(QString path, QString tag) {
    _db->addTag(path, tag);
    updateSelectedImage();
    updateFilteredImages();
}

void AppState::removeTag(QString path, QString tag) {
    _db->removeTag(path, tag);
    updateSelectedImage();
    updateFilteredImages();
}

void AppState::setDescription(QString path, QString description) {
    _db->setDescription(path, description);
    // We do not update because change is instantanious in QPlainTextEdit
}

void AppState::setCrop(const QString& path, std::optional<QRect> crop) {
    _db->setCrop(path, crop);
    updateSelectedImage();
    updateFilteredImages();
}

void AppState::addFilter(const Filter& f) {
    _filters.push_back(f); updateFilteredImages();
}

void AppState::removeFilter(const Filter& f) {
    _filters.erase(std::remove_if(_filters.begin(), _filters.end(), [&](Filter& f1){return filterEquals(f, f1);}));
    updateFilteredImages();
}

void AppState::updateFilteredImages() {
    _filteredImages = _db->query(_filters);
    emit onFilteredImagePathsChanged();
}

void AppState::updateSelectedImage() {
    if (_selectedImage.has_value()) _selectedImage = _db->queryByPath(_selectedImage->path);
    emit onSelectedImageChanged();
}
