#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "previewimg.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->mainImg->setPath("C:/Users/Eidos/Pictures/backiee-138908-landscape-scaled.jpg");
    for (int i = 0; i < 5; ++i)
        ui->carouselLayout->addWidget(new PreviewImg("C:/Users/Eidos/Pictures/backiee-138908-landscape-scaled.jpg"));
    ui->carouselLayout->addStretch();
}

MainWindow::~MainWindow()
{
    delete ui;
}

