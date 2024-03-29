#include "db.h"
#include <QFile>
#include <QStandardPaths>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>

// JSON file database

SQLiteDatabase::SQLiteDatabase(const QString& dbPath) {
    _db = QSqlDatabase::addDatabase("QSQLITE");
    _db.setDatabaseName(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/" + dbPath);

    if (!_db.open()) {
        qDebug() << "Could not connect to SQLite database!";
        return;
    }

    auto query = QSqlQuery();
    query.prepare(R"'(
        CREATE TABLE IF NOT EXISTS Images (
            path VARCHAR NOT NULL PRIMARY KEY,
            description VARCHAR DEFAULT "",
            rating INT NOT NULL,
            cropX INT,
            cropY INT,
            cropW INT,
            cropH INT
        );
    )'");
    if (!query.exec()) qDebug() << "SQL Database Error:" << query.lastError();
    query.prepare(R"'(
        CREATE TABLE IF NOT EXISTS Tags (
            name VARCHAR,
            path VARCHAR REFERENCES Images (path) ON DELETE CASCADE,
            PRIMARY KEY (name, path)
        );
    )'");
    if (!query.exec()) qDebug() << "SQL Database Error:" << query.lastError();

    qDebug() << "SQLite connection successful";
}

std::vector<Image> sqlToImages(QSqlQuery& q) {
    int pathRow = q.record().indexOf("Images.path");
    int descriptionRow = q.record().indexOf("Images.description");
    int ratingRow = q.record().indexOf("Images.rating");
    int cropXRow = q.record().indexOf("Images.cropX");
    int cropYRow = q.record().indexOf("Images.cropY");
    int cropWRow = q.record().indexOf("Images.cropW");
    int cropHRow = q.record().indexOf("Images.cropH");
    int tagNameRow = q.record().indexOf("Tags.name");

    auto l = std::vector<Image>();

    while (q.next()) {
        QString path = q.value(pathRow).toString();

        if (l.empty() || l.back().path != path) {
            auto& img = l.emplace_back(path);
            img.description = q.value(descriptionRow).isNull() ? "" : q.value(descriptionRow).toString();
            img.rating = q.value(ratingRow).toInt();

            int cropX = q.value(cropXRow).isNull() ? -1 : q.value(cropXRow).toInt();
            int cropY = q.value(cropYRow).isNull() ? -1 : q.value(cropYRow).toInt();
            int cropW = q.value(cropWRow).isNull() ? -1 : q.value(cropWRow).toInt();
            int cropH = q.value(cropHRow).isNull() ? -1 : q.value(cropHRow).toInt();
            if (cropX != -1 && cropY != -1 && cropW != -1 && cropH != -1)
                img.crop = QRect(cropX, cropY, cropW, cropH);
        }
        if (!q.value(tagNameRow).isNull()) l.back().tags.push_back(q.value(tagNameRow).toString());
    }

    return l;
}

std::vector<Image> SQLiteDatabase::query(const std::vector<Filter>& filters) {
    auto query = QSqlQuery();
    auto queryStr = QString("SELECT * FROM Images LEFT JOIN Tags USING (path) WHERE 1");
    auto sqlFilters = std::vector<QVariant>();

    for (size_t i = 0; i < filters.size(); ++i) {
        const Filter& f = filters[i];
        auto filterLabel = ":filter" + QString::number(i);
        try {
            sqlFilters.push_back(std::get<RatingFilter>(f).min);
            queryStr += " AND rating >= (" + filterLabel + ")";
        } catch (...) {}
        try {
            sqlFilters.push_back(std::get<TagFilter>(f).tag);
            queryStr += " AND ((" + filterLabel + "), path) IN Tags";
        } catch (...) {}
    }

    query.prepare(queryStr);
    for (size_t i = 0; i < sqlFilters.size(); ++i)
        query.bindValue(":filter" + QString::number(i), sqlFilters[i]);
    if (!query.exec()) qDebug() << "SQL Database Error:" << query.lastError();
    return sqlToImages(query);
}

Image SQLiteDatabase::queryByPath(QString path) {
    auto query = QSqlQuery();
    query.prepare("SELECT * FROM Images LEFT JOIN Tags USING (path) WHERE path = (:path)");
    query.bindValue(":path", path);
    if (!query.exec()) qDebug() << "SQL Database Error:" << query.lastError();
    return sqlToImages(query)[0];
}

void SQLiteDatabase::addImage(const Image& img) {
    auto query = QSqlQuery();
    query.prepare("INSERT INTO Images VALUES ("
                    "(:path),"
                    "(:description),"
                    "(:rating),"
                    "(:cornerTL),"
                    "(:cornerTR),"
                    "(:cornerBR),"
                    "(:cornerBL)"
                  ")");
    query.bindValue(":path", img.path);
    query.bindValue(":description", img.description);
    query.bindValue(":rating", img.rating);
    query.bindValue(":cornerTL", QVariant::fromValue(QMetaType::QString));
    query.bindValue(":cornerTR", QVariant::fromValue(QMetaType::QString));
    query.bindValue(":cornerBR", QVariant::fromValue(QMetaType::QString));
    query.bindValue(":cornerBL", QVariant::fromValue(QMetaType::QString));

    if (!query.exec()) qDebug() << "SQL Database Error:" << query.lastError();
}

void SQLiteDatabase::removeImage(const QString& path) {
    auto query = QSqlQuery();
    query.prepare("DELETE FROM Images WHERE path = (:path)");
    query.bindValue(":path", path);
    if (!query.exec()) qDebug() << "SQL Database Error:" << query.lastError();
}

void SQLiteDatabase::setRating(QString path, int rating) {
    auto query = QSqlQuery();
    query.prepare("UPDATE Images SET rating = (:rating) WHERE path = (:path)");
    query.bindValue(":path", path);
    query.bindValue(":rating", rating);
    if (!query.exec()) qDebug() << "SQL Database Error:" << query.lastError();
}

void SQLiteDatabase::addTag(QString path, QString tag) {
    auto query = QSqlQuery();
    query.prepare("INSERT INTO Tags VALUES ((:tag), (:path))");
    query.bindValue(":path", path);
    query.bindValue(":tag", tag);
    if (!query.exec()) qDebug() << "SQL Database Error:" << query.lastError();
}

void SQLiteDatabase::removeTag(QString path, QString tag) {
    auto query = QSqlQuery();
    query.prepare("DELETE FROM Tags WHERE name = (:tag) and path = (:path)");
    query.bindValue(":path", path);
    query.bindValue(":tag", tag);
    if (!query.exec()) qDebug() << "SQL Database Error:" << query.lastError();
}

void SQLiteDatabase::setDescription(QString path, QString description) {
    auto query = QSqlQuery();
    query.prepare("UPDATE Images SET description = (:description) WHERE path = (:path)");
    query.bindValue(":path", path);
    query.bindValue(":description", description);
    if (!query.exec()) qDebug() << "SQL Database Error:" << query.lastError();
}

void SQLiteDatabase::setCrop(const QString& path, std::optional<QRect> crop) {
    auto query = QSqlQuery();
    query.prepare("UPDATE Images "
                  "SET cropX = (:cropX), cropY = (:cropY), cropW = (:cropW), cropH = (:cropH) "
                  "WHERE path = (:path)");
    query.bindValue(":cropX", crop->x());
    query.bindValue(":cropY", crop->y());
    query.bindValue(":cropW", crop->width());
    query.bindValue(":cropH", crop->height());
    query.bindValue(":path", path);
    if (!query.exec()) qDebug() << "SQL Database Error:" << query.lastError();
}




// Vector memory database

std::vector<Image> VectorDatabase::query(const std::vector<Filter>& filters) {
    std::vector<Image> l;
    for (const auto& img : _images) {
        if (std::all_of(filters.cbegin(), filters.cend(), [&](const Filter& f){ return matchesFilter(img, f); }))
            l.push_back(img);
    }
    return l;
}

Image VectorDatabase::queryByPath(QString path) {
    return queryRefByPath(path);
}

Image& VectorDatabase::queryRefByPath(QString path) {
    for (Image& img : _images) if (img.path == path) return img;
    throw std::runtime_error("Path not found");
}

void VectorDatabase::addImage(const Image& img) {
    if (std::any_of(_images.begin(), _images.end(), [&](Image& x){return x.path == img.path;})) return;
    _images.push_back(img);
}

void VectorDatabase::removeImage(const QString& path) {
    _images.erase(std::remove_if(_images.begin(), _images.end(), [&](Image& img){return img.path == path;}));
}

void VectorDatabase::setRating(QString path, int rating) {
    queryRefByPath(path).rating = rating;
}

void VectorDatabase::addTag(QString path, QString tag) {
    queryRefByPath(path).tags.push_back(tag);
}

void VectorDatabase::removeTag(QString path, QString tag) {
    auto& tags = queryRefByPath(path).tags;
    tags.erase(std::remove_if(tags.begin(), tags.end(), [&](QString& t){return t == tag;}));
}

void VectorDatabase::setDescription(QString path, QString description) {
    queryRefByPath(path).description = description;
}

void VectorDatabase::setCrop(const QString& path, std::optional<QRect> crop) {
    auto& ref = queryRefByPath(path);
    ref.crop = crop;
}
