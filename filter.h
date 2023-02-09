#ifndef FILTER_H
#define FILTER_H

#include <QWidget>
#include "image.h"
#include "tag.h"

struct TagFilter { QString tag; };
struct RatingFilter { int min = 0; };
using Filter = std::variant<TagFilter, RatingFilter>;

inline bool filterEquals(const Filter& f1, const Filter& f2) {
    try {
        return std::get<TagFilter>(f1).tag == std::get<TagFilter>(f2).tag;
    } catch (...) {}
    try {
        return std::get<RatingFilter>(f1).min == std::get<RatingFilter>(f2).min;
    } catch (...) {}
    return false;
}

inline bool matchesFilter(const Image& img, const Filter& f) {
    // On verifie que l'image contient bien le tag
    try {
        QString tag = std::get<TagFilter>(f).tag;
        if (std::find_if(img.tags.begin(), img.tags.end(), [tag](const QString& t){ return tag.toLower() == t.toLower(); }) == img.tags.end())
            return false;
    } catch (...) {}

    // On verifie que l'image a un rating suffisamment elevé
    try {
        int minRating = std::get<RatingFilter>(f).min;
        if (minRating > img.rating) return false;
    } catch (...) {}

    return true;
}

inline Tag* filterToWidget(const Filter& f, QWidget* parent) {
    try {
        auto tag = std::get<TagFilter>(f);
        return new Tag(QString("#").append(tag.tag), parent);
    } catch (...) {}
    try {
        auto rating = std::get<RatingFilter>(f);

        auto stars = QString("");
        for (int i = 0; i < rating.min; ++i) stars = stars.append("⭐");
        return new Tag(QString("Min. ").append(stars), parent, "#AB6CDC", "#CC96F5");
    } catch (...) {}
    throw std::runtime_error("No variant matches???");
}


#endif // FILTER_H
