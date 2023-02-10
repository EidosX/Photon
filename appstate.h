#ifndef APPSTATE_H
#define APPSTATE_H

#include <QDebug>
#include <QObject>
#include "db.h"

// AppState holds all the state of the frontend application.
// Whenever the user clicks on an image to select it, or adds a filter to find
// certain images, every event is handled by AppState which will store any
// required data for the frontend application (e.g the current images in the
// carousel, which change according to the database and the given filters).
//
// This class uses an underlying Database interface, which stores
// data we want to save, i.e the photos and their metadata.
class AppState : public QObject
{
    Q_OBJECT
public:
    AppState(std::unique_ptr<Database> db, QObject *parent = nullptr);

    // Returns the currently selected image.
    const std::optional<Image>& getSelectedImage();

    // Returns the current selection of images from the database
    // that match the currently given filters.
    const std::vector<Image>& getFilteredImages();

signals:
    // Emitted whenever the filtered images or their metadata may have changed
    void onFilteredImagePathsChanged();

    // Emitted whenever the selected image or its metadata may have changed
    void onSelectedImageChanged();

public slots:
    // Adds an image to the database.
    void addImage(const Image& img);

    // Removes the image with the given path from the database
    void removeImage(const QString& path);

    // Sets the currently selected image to the one with the given path
    void setSelectedImage(QString path);

    // Sets the rating of the currently selected image. Rating must be between 0 and 5
    void setSelectedImageRating(int rating);

    // Adds a tag to the image with the given path
    void addTag(QString path, QString tag);

    // Removes a tag from the image with the given path
    void removeTag(QString path, QString tag);

    // Sets the description for the image with the given path
    void setDescription(QString path, QString description);

    // Sets the crop rectangle for the image with the given path
    void setCrop(const QString& path, std::optional<QRect> crop);

    // Adds a filter to the image selection
    void addFilter(const Filter& f);

    // Remove a filter from the image selection
    void removeFilter(const Filter& f);

private:
    // Convenience method to make sure all filtered images are up to date
    // and to notifiy signal listeners
    void updateFilteredImages();

    // Convenience method to make sure the currently selected image is up to date
    // to to notify signal listeners
    void updateSelectedImage();

    std::vector<Filter> _filters;
    std::unique_ptr<Database> _db;
    std::vector<Image> _filteredImages;
    std::optional<Image> _selectedImage;
};

#endif // APPSTATE_H
