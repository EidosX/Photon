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
    while (QWidget* w = ui->carouselScroll->findChild<PreviewImg*>())
        delete w;

    for (const auto& img : _appState.getFilteredImages()) {
        auto* w = new PreviewImg(img, _appState, this);
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
