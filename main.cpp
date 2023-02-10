#include "mainwindow.h"

#include <QApplication>
#include "appstate.h"
#include "db.h"

// This is where we initiliase the front end, as well as the app state
// and the database.
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // We can swap the concrete type of the database to something else
    // very easily (for example, we can put an in-memory VectorDatabase).
    // This is very useful if we ever want to change our database backend,
    // for example to support cloud storage.
    auto db = std::make_unique<SQLiteDatabase>("photon.db");
    auto appState = AppState(std::move(db));

    auto w = MainWindow(appState);
    w.show();
    return a.exec();
}
