#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "previewimg.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->mainImg->setPath("C:/Users/Eidos/Pictures/backiee-138908-landscape-scaled.jpg");
    ui->carouselLayout->addWidget(new PreviewImg("C:/Users/Eidos/Pictures/backiee-138908-landscape-scaled.jpg"));
    ui->carouselLayout->addWidget(new PreviewImg("C:/Users/Eidos/Desktop/cc.png"));
    ui->carouselLayout->addWidget(new PreviewImg("C:/Users/Eidos/Pictures/backiee-138908-landscape-scaled.jpg", true));
    ui->carouselLayout->addWidget(new PreviewImg("C:/Users/Eidos/Desktop/cc.png"));
    ui->carouselLayout->addStretch();
}

MainWindow::~MainWindow()
{
    delete ui;
}

