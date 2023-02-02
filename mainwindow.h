#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "appstate.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(AppState& appState, QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void reloadCarousel();

private:
    Ui::MainWindow *ui;
    AppState& _appState;
};
#endif // MAINWINDOW_H
