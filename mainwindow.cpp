#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "previewimg.h"
#include "mainimg.h"
#include <QDragEnterEvent>
#include <QInputDialog>
#include <QMessageBox>
#include <QMimeData>
#include "star.h"
#include "tag.h"

MainWindow::MainWindow(AppState& appState, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), _appState(appState)
{
    ui->setupUi(this);
    setAcceptDrops(true);

    connect(&appState, &AppState::onSelectedImageChanged, ui->mainImg, [this](){
        if (_appState.getSelectedImage().has_value()){
            ui->mainImg->setPath(_appState.getSelectedImage()->path);
            ui->descField->setPlainText(_appState.getSelectedImage()->description);
        } else ui->mainImg->setPath("");
    });

    connect(&appState, &AppState::onFilteredImagePathsChanged, this, &MainWindow::reloadCarousel);

    auto starWidgets = ui->starsWidget->findChildren<Star*>();
    for (int i = 1; i <= 5; ++i) {
        auto* star = starWidgets[i-1];
        connect(&appState, &AppState::onSelectedImageChanged, star, [this, i, star](){
            int currentStarsCount = _appState.getSelectedImage().has_value()
                    ? _appState.getSelectedImage().value().rating : 0;
            star->enable(i <= currentStarsCount);
        });

        connect(star, &Star::clicked, star, [this, i](){
            if (!_appState.getSelectedImage().has_value()) return;
            int newRating = _appState.getSelectedImage()->rating == i ? 0 : i;
            _appState.setSelectedImageRating(newRating);
        });
    }

    auto placeholder = new QWidget(this);
    placeholder->setMinimumHeight(PreviewImg::HEIGHT);
    ui->carouselLayout->addWidget(placeholder);
    ui->carouselLayout->addStretch(0);

    reloadCarousel();

    auto* addTagBtn = new TagButton("Add Tag");
    connect(addTagBtn, &QPushButton::clicked, &_appState, [this](){
        if (!_appState.getSelectedImage().has_value()) return;
        auto tagName = QInputDialog::getText(this, "Add Tag", "Enter tag name: ");
        if (tagName.size() < 1) {
            (new QMessageBox(QMessageBox::Critical, "Error", "Tag name must not be empty"))->show();
            return;
        }
        _appState.addTag(_appState.getSelectedImage()->path, tagName);
    });
    ui->currTagsScrollLayout->insertWidget(0, addTagBtn);

    connect(&appState, &AppState::onSelectedImageChanged, ui->currTagsScrollContents, [this](){
        while (auto* w = ui->currTagsScrollContents->findChild<Tag*>()) delete w;
        if (!_appState.getSelectedImage().has_value()) return;
        for (auto& t : _appState.getSelectedImage()->tags) {
            auto* tag = new Tag(t, this);
            connect(tag, &Tag::delBtnClicked, &_appState, [this, t](){
                _appState.removeTag(_appState.getSelectedImage()->path, t);
            });
            ui->currTagsScrollLayout->insertWidget(ui->currTagsScrollLayout->count() - 2, tag);
        }
    });
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
        auto* w = new PreviewImg(newSelection[j].path, this);
        connect(&_appState, &AppState::onSelectedImageChanged, w, [this, w](){
            w->setSelected(_appState.getSelectedImage().has_value() && w->getPath() == _appState.getSelectedImage()->path);
        });
        connect(w, &PreviewImg::clicked, w, [this, w](){
            if (!w->isSelected()) _appState.setSelectedImage(w->getPath());
            else _appState.setSelectedImage({});
        });
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
    for (const auto& path : paths){
        // Qt returns /C:/Users/... instead of C:/Users... on Windows
        auto p = path.path();
        p = p[2] == ':' ? p.sliced(1) : p;
        _appState.addImage(Image(p));
    }
}
