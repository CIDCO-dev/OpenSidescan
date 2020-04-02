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
#include "channelpropertieswindow.h"

#include "sidescanfile.h"
#include "project.h"

#include "parameterscvCreateTrainingSamples.h"

#include "boolwithmutex.h"


//#include "progressdialogtrainingsamples.h"

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

    Project * getProject() {return currentProject;};

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
    void actionSaveObjectImages();

    void actionProjectProperties();

    void actionExportTrainingObjectSamples();

    void actionExportKmlFile();

    void actionExportCsvFile();


    void fileSelected(const QItemSelection & selection);
    void objectSelected(GeoreferencedObject * object);

    void refreshTabs();

    void removeSidescanFileFromProject( SidescanFile * file );

    void addFileToProjectWindow( SidescanFile * file);

    void tabChanged( int index );


private slots:

    void on_actionProperties_triggered();

private:

    void buildUI();
    bool promptProject();
    void refreshProjectUI();
    void selectImageTab(GeoreferencedObject * object);

    void createAndSaveTrainingObjectSamples( const QString & folder, const ParameterscvCreateTrainingSamples & parameters );

    Ui::MainWindow          * ui;
    QTabWidget              * tabs;

    ProjectWindow           * projectWindow;
    FilePropertiesWindow    * fileInfo;
    InventoryWindow         * inventoryWindow;
    ChannelPropertiesWindow * channelInfo;

    SidescanImage        * selectedImage  = NULL;
    SidescanFile         * selectedFile   = NULL;
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

    ParameterscvCreateTrainingSamples parameterscvCreateTrainingSamples;

    QString folderCreateTrainingSamples;
};

#endif // MAINWINDOW_H
