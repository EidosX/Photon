#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "previewimg.h"
#include "mainimg.h"
#include <QDragEnterEvent>
#include <QMimeData>
#include "Star.h"

MainWindow::MainWindow(AppState& appState, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), _appState(appState)
{
    ui->setupUi(this);
    setAcceptDrops(true);
    ui->mainHorizontalLayout->insertWidget(0, new MainImg(_appState, this));

    connect(&appState, &AppState::onFilteredImagePathsChanged, this, &MainWindow::reloadCarousel);

    for (int i = 1; i <= 5; ++i)
        ui->starsLayout->addWidget(new Star(_appState, i, this));

    auto placeholder = new QWidget(this);
    placeholder->setMinimumHeight(PreviewImg::HEIGHT);
    ui->carouselLayout->addWidget(placeholder);
    ui->carouselLayout->addStretch(0);

    reloadCarousel();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::reloadCarousel() {
    auto oldWidgets = ui->carouselScroll->findChildren<PreviewImg*>();
    auto newSelection = _appState.getFilteredImages();
    size_t i = 0;

    // Leave all the images that are still the same
    while (i < (size_t) oldWidgets.size() && i < newSelection.size()){
        if (oldWidgets[i]->getPath() != newSelection[i].path) break;
        ++i;
    }
    // Once an image is wrong, delete everything afterwards
    // Its size will change so we will need to recalculate the layout anyway
    for (size_t j = i; j < (size_t) oldWidgets.size(); ++j) delete oldWidgets[j];

    // Fill the missing images
    for (size_t j = i; j < newSelection.size(); ++j) {
        auto* w = new PreviewImg(newSelection[j].path, _appState, this);
        ui->carouselLayout->insertWidget(ui->carouselLayout->count()-2, w);
    }

    emit _appState.onSelectedImageChanged();
}

bool isFileValid(const QUrl& url) {
    if (!url.isValid()) return false;
    if (!url.isLocalFile()) return false;

    for (const auto& extension : {".png", ".jpg"})
        if (url.fileName().endsWith(extension)) return true;
    return false;
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event) {
    if (!event->mimeData()->hasUrls()) return;
    for (const auto& url : event->mimeData()->urls())
        if (!isFileValid(url)) return;
    event->acceptProposedAction();
}

void MainWindow::dropEvent(QDropEvent *event) {
    auto paths = event->mimeData()->urls();
    for (const auto& path : paths)
        _appState.addImage(Image(path.path().sliced(1)));
}
