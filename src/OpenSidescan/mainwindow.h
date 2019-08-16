#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QScrollArea>
#include <QLabel>

#include "sidescanimager.h"
#include "opencvhelper.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void actionOpen();
    void actionQuit();
    void actionAbout();
    void saveImage();

private:
    void updateUI();
    void buildUI();

    Ui::MainWindow * ui;
    QTabWidget  *    tabs;

    QScrollArea *    scrollArea;
    QLabel *         imageLabel;

    std::list<cv::Mat*> * images;
};

#endif // MAINWINDOW_H
