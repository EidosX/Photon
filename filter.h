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
    } catch (const std::bad_variant_access& ex) {}
    try {
        return std::get<RatingFilter>(f1).min == std::get<RatingFilter>(f2).min;
    } catch (const std::bad_variant_access& ex) {}
    return false;
}

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

inline Tag* filterToWidget(const Filter& f, QWidget* parent) {
    try {
        auto tag = std::get<TagFilter>(f);
        return new Tag(QString("#").append(tag.tag), parent);
    } catch (const std::bad_variant_access& ex) {}
    try {
        auto rating = std::get<RatingFilter>(f);

        auto stars = QString("");
        for (int i = 0; i < rating.min; ++i) stars = stars.append("⭐");
        return new Tag(QString("Min. ").append(stars), parent);
    } catch (const std::bad_variant_access& ex) {}
    throw std::runtime_error("No variant matches???");
}


#endif // FILTER_H
