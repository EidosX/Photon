#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDatabase>
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

class SQLiteDatabase : public Database {
    Q_OBJECT
public:
    SQLiteDatabase(const QString& dbPath);
    std::vector<Image> query(const std::vector<Filter>& filters) override;
    Image queryByPath(QString path) override;

public slots:
    void addImage(const Image& img) override;
    void removeImage(const QString& path) override;
    void setRating(QString path, int rating) override;
    void addTag(QString path, QString tag) override;
    void removeTag(QString path, QString tag) override;
    void setDescription(QString path, QString description) override;

private:
    QSqlDatabase _db;

};

class VectorDatabase : public Database {
    Q_OBJECT
public:
    std::vector<Image> query(const std::vector<Filter>& filters) override;
    Image queryByPath(QString path) override;
    Image& queryRefByPath(QString path);

public slots:
    void addImage(const Image& img) override;
    void removeImage(const QString& path) override;
    void setRating(QString path, int rating) override;
    void addTag(QString path, QString tag) override;
    void removeTag(QString path, QString tag) override;
    void setDescription(QString path, QString description) override;

private:
    std::vector<Image> _images;
};


#endif // DATABASE_H
