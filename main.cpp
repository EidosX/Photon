#include "mainwindow.h"

#include <QApplication>
#include "appstate.h"
#include "db.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    auto db = std::make_unique<SQLiteDatabase>("photon.db");
    auto appState = AppState(std::move(db));

    auto w = MainWindow(appState);
    w.show();
    return a.exec();
}
