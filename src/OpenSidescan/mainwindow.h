#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QScrollArea>
#include <QLabel>

#include "sidescanimage.h"
#include "sidescanimager.h"
#include "opencvhelper.h"

#include "projectwindow.h"
#include "filepropertieswindow.h"
#include "inventorywindow.h"

#include "sidescanfile.h"
#include "project.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void updateSelectedFile(SidescanFile * file);
    void refreshObjectInventory();
    void dummy(){ std::cout << "POOP" << std::endl;}

public slots:
    void createProject();

    void actionOpen();
    void actionQuit();
    void actionAbout();
    void actionFindObjects();
    void actionCleanImage();

    void fileSelected(const QItemSelection & selection);
    void refreshTabs();

private:

    void buildUI();

    Ui::MainWindow * ui;
    QTabWidget     * tabs;

    ProjectWindow        * projectWindow;
    FilePropertiesWindow * fileInfo;
    InventoryWindow      * inventoryWindow;


    SidescanFile         * selectedFile;

    Project              * currentProject = NULL;

    //Detection parameters
    int    fastThresholdValue = 100;
    int    fastTypeValue =cv::FastFeatureDetector::TYPE_9_16;
    bool   fastNonMaxSuppressionValue = false;
    int    dbscanEpsilonValue = 50;
    int    dbscanMinPointsValue = 10;
    int    mserDeltaValue = 5;
    int    mserMinimumAreaValue=60;
    int    mserMaximumAreaValue= 14400;
    double mserMaximumVariationValue=0.25;
    double mserMinimumDiversityValue=0.2;
    int    mserMaximumEvolutionValue=200;
    double mserAreaThresholdValue=1.01;
    double mserMinimumMarginValue=0.003;
    int    mserEdgeBlurValue = 5;
    bool   showFeatureMarkersValue = false;
    bool   mergeOverlappingBoundingBoxesValue = true;
};

#endif // MAINWINDOW_H
