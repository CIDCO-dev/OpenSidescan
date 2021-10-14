#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QScrollArea>
#include <QLabel>
#include <QAction>

#include "sidescan/sidescanimage.h"
#include "sidescan/sidescanimager.h"
#include "utilities/opencvhelper.h"

#include "ui/docks/projectwindow/projectwindow.h"
#include "ui/docks/filepropertieswindow.h"
#include "ui/docks/inventorywindow.h"
#include "ui/docks/channelpropertieswindow.h"
#include "ui/docks/vesselgeometrywindow.h"
#include "ui/mainwindow/monitorthread.h"

#include "sidescan/sidescanfile.h"
#include "project/project.h"

#include "ui/training/parameterscvCreateTrainingSamples.h"

#include "refactorme/boolwithmutex.h"


namespace Ui {
class MainWindow;

}

class MonitorThread;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void updateSelectedFile(SidescanFile * file);

    Project * getProject() {return currentProject;}

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

    void actionExportHTML();

    void actionExportTrainingObjectSamples();

    void actionExportKmlFile();

    void actionExportCsvFile();
    
    void actionExportHitsFile();

    void actionExportYolo();


    void fileSelected(const QItemSelection & selection);
    void objectSelected(InventoryObject * object);

    void refreshTabs();

    void removeSidescanFileFromProject( SidescanFile * file );

    void addFileToProjectWindow( SidescanFile * file);

    void tabChanged( int index );

    void monitorProgress(QString progress);


private slots:

    void monitorActionTriggered();

protected:

    void buildUI();
    bool promptProject();
    void refreshProjectUI();
    void selectImageTab(InventoryObject * object);

    void createAndSaveTrainingObjectSamples( const QString & folder, const ParameterscvCreateTrainingSamples & parameters );

    Ui::MainWindow          * ui;
    QTabWidget              * tabs;

    ProjectWindow           * projectWindow;
    VesselGeometryWindow    * vesselWindow;
    FilePropertiesWindow    * fileInfo;
    InventoryWindow         * inventoryWindow;
    ChannelPropertiesWindow * channelInfo;

    SidescanImage        * selectedImage  = nullptr;
    SidescanFile         * selectedFile   = nullptr;
    Project              * currentProject = nullptr;

    MonitorThread        * monitorThread = nullptr;
    QAction * monitorAction = nullptr;

    //Detection parameters
    int    fastThresholdValue                   = 100;
    int    fastTypeValue                        = cv::FastFeatureDetector::TYPE_9_16;
    bool   fastNonMaxSuppressionValue           = false;
    int    dbscanEpsilonValue                   = 50;
    int    dbscanMinPointsValue                 = 10;
    int    mserDeltaValue                       = 5;
    int    mserMinimumAreaValue                 = 60;
    int    mserMaximumAreaValue                 = 14400;
    bool   mergeOverlappingBoundingBoxesValue   = true;

    ParameterscvCreateTrainingSamples parameterscvCreateTrainingSamples;

    QString folderCreateTrainingSamples;
};

#endif // MAINWINDOW_H
