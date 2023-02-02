#include "mainwindow.h"

#include <QApplication>
#include "appstate.h"
#include "db.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    auto db = std::make_unique<VectorDatabase>();
    const std::vector<QString> exampleImages = {
      "C:/Users/Eidos/Pictures/backiee-138908-landscape-scaled.jpg",
      "C:/Users/Eidos/Pictures/cc.png",
      "C:/Users/Eidos/Pictures/Render B11 Post 1.png",
      "C:/Users/Eidos/Pictures/girl-walking-with-headphones-anime-digital-art-9q.jpg",
      "C:/Users/Eidos/Pictures/download.png",
      "C:/Users/Eidos/Pictures/Screenshots/Screenshot_20230118_161438.png"
    };
    for (auto& path : exampleImages) {
        auto img = Image();
        img.path = path;
        img.rating = 4;
        db->addImage(img);
    }

    auto appState = AppState(std::move(db));

    auto w = MainWindow(appState);
    w.show();
    return a.exec();
}
