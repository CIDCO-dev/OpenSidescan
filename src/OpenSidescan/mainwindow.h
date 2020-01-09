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

    // TODO: ? Put preprocessor condition so as not to use when not building for test?
    friend class testGUI;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void updateSelectedFile(SidescanFile * file);


    void refreshObjectInventory(){
        inventoryWindow->refreshInventoryTable();
        refreshTabs();
    }

public slots:
    void actionCreate();
    void actionOpen();
    void actionClose();
    void actionSave();
    void actionSaveAs();
    void actionImport();
    void actionQuit();
    void actionAbout();
    void actionFindObjects();
    void actionExportKmlFile();

    void fileSelected(const QItemSelection & selection);
    void objectSelected(GeoreferencedObject * object);

    void refreshTabs();

    void removeSidescanFileFromProject( SidescanFile * file );

    void addFileToProjectWindow( SidescanFile * file);


private:

    void buildUI();
    bool promptProject();
    void refreshProjectUI();
    void selectImageTab(GeoreferencedObject * object);

    Ui::MainWindow       * ui;
    QTabWidget           * tabs;

    ProjectWindow        * projectWindow;
    FilePropertiesWindow * fileInfo;
    InventoryWindow      * inventoryWindow;

    SidescanFile         * selectedFile;
    Project              * currentProject = NULL;

    //Detection parameters
    int    fastThresholdValue                   = 100;
    int    fastTypeValue                        = cv::FastFeatureDetector::TYPE_9_16;
    bool   fastNonMaxSuppressionValue           = false;
    int    dbscanEpsilonValue                   = 50;
    int    dbscanMinPointsValue                 = 10;
    int    mserDeltaValue                       = 5;
    int    mserMinimumAreaValue                 = 60;
    int    mserMaximumAreaValue                 = 14400;
    bool   showFeatureMarkersValue              = false;
    bool   mergeOverlappingBoundingBoxesValue   = true;

};

#endif // MAINWINDOW_H
