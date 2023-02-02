#include "appstate.h"


AppState::AppState(std::unique_ptr<Database> db, QObject *parent)
    : QObject{parent}, _db(std::move(db))
{
    _filteredImages = _db->query({});
}
