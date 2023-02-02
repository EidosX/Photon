#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "previewimg.h"
#include "mainimg.h"

MainWindow::MainWindow(AppState& appState, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), _appState(appState)
{
    ui->setupUi(this);
    ui->mainHorizontalLayout->insertWidget(0, new MainImg(_appState, this));

    connect(&appState, &AppState::onFilteredImagePathsChanged, this, &MainWindow::reloadCarousel);

    reloadCarousel();
    appState.setSelectedImage(appState.getFilteredImages()[3]);
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
        ui->carouselLayout->addWidget(w);
    }
    ui->carouselLayout->addStretch(0);
    emit _appState.onSelectedImageChanged();
}
