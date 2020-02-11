#include <algorithm>    // std::find

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDir>
#include <QFileDialog>
#include <QTabWidget>
#include <QProgressDialog>
#include <QDockWidget>
#include <string>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QScrollBar>

#include <QThread>

#include <QDebug>

#include <iostream>
#include "imagetab.h"
#include "aboutdialog.h"
#include "projectwindow.h"
#include "detectionwindow.h"
#include "inventorywindow.h"

#include "workerimportsidescanfiles.h"
#include "workeropenproject.h"

#include "parameterscvCreateTrainingSamples.h"

#include "trainingsampleswindow.h"

#include "../../src/thirdParty/MBES-lib/src/utils/StringUtils.hpp"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    tabs(new QTabWidget),
    currentProject(NULL),
    fileInfo(NULL)
{
    ui->setupUi(this);
    buildUI();

    statusBar()->showMessage("Load a sidescan files using File->Open menu");
}

void MainWindow::buildUI(){
    this->setTabPosition(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea|Qt::BottomDockWidgetArea,QTabWidget::North);

    projectWindow = new ProjectWindow(this);
    this->addDockWidget(Qt::LeftDockWidgetArea,projectWindow);
    connect(ui->actionShowProjectWindow,&QAction::triggered,projectWindow,&ProjectWindow::show);
    connect(projectWindow, &ProjectWindow::removeFileFromProjectRequest, this, &MainWindow::removeSidescanFileFromProject);

    fileInfo = new FilePropertiesWindow(this);
    this->addDockWidget(Qt::LeftDockWidgetArea,fileInfo);
    connect(ui->actionShowFilePropertiesWindow,&QAction::triggered,fileInfo,&FilePropertiesWindow::show);

    inventoryWindow = new InventoryWindow(this);
    this->addDockWidget(Qt::BottomDockWidgetArea,inventoryWindow);
    connect(ui->actionShowObjectInventoryWindow,&QAction::triggered,inventoryWindow,&InventoryWindow::show);
    connect(inventoryWindow,&InventoryWindow::objectSelected,this,&MainWindow::objectSelected);




    actionCreate();

    //center
    this->setCentralWidget(tabs);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete projectWindow;
}

void MainWindow::actionCreate(){

    if(promptProject()){

        if(currentProject){
            actionClose();
        }

        currentProject = new Project();

        refreshProjectUI();
    }
}

bool MainWindow::promptProject(){
    if(this->currentProject){
        //Prompt user if he wants to override current project
        QMessageBox msgBox;
        msgBox.setText("There's already an active project. Are you sure you want to continue?");
        msgBox.setInformativeText("All unsaved changes will be lost.");
        msgBox.setStandardButtons(QMessageBox::Cancel | QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Cancel);
        int ret = msgBox.exec();

        if(ret == QMessageBox::Ok){
            return true;
        }

        return false;
    }

    return true;
}

/**
 * @brief MainWindow::refreshProjectUI
 * Refreshes the UI after project load or creation
 */
void MainWindow::refreshProjectUI(){

    projectWindow->setProject(currentProject);

    inventoryWindow->setProject(currentProject);

    selectedFile = NULL;

    updateSelectedFile(selectedFile);

    //Set window title
    std::string title = (currentProject)?  (currentProject->getFilename().size() > 0 ? currentProject->getFilename() : "New Project")  : "No active project";
    this->setWindowTitle(QString::fromStdString(title));
}

void MainWindow::actionImport(){

    // QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Import Sidescan Files"), "", tr("Sidescan Files (*.xtf)"));
	
	QStringList fileNames = QFileDialog::getOpenFileNames( this, 
															tr("Import Sidescan Files"), 
															"", 
															tr("Sidescan Files (*.xtf)"), 
															nullptr,
															QFileDialog::DontUseNativeDialog );


    if(fileNames.size() <= 0)
        return;

    statusBar()->showMessage("Loading sidescan data...");

    //Load file -----------------------------------*
    unsigned int nbFiles = fileNames.size();

    QProgressDialog progress("Loading files...", QString(), 0, nbFiles*2, this);
    progress.setWindowModality( Qt::WindowModal );
    progress.setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint | Qt::WindowTitleHint);

    progress.setValue(0);
    progress.setMinimumDuration( 0 );

    SidescanFile * lastFile = NULL; //used to "open" a file after the import

    QThread * workerThread = new QThread( this );

    WorkerImportSidescanFiles * worker = new WorkerImportSidescanFiles( fileNames, projectWindow, &lastFile );

    worker->moveToThread(workerThread);

    connect( workerThread, &QThread::finished, worker, &WorkerImportSidescanFiles::deleteLater );
    connect( workerThread, &QThread::started, worker, &WorkerImportSidescanFiles::doWork );


    connect( worker, &WorkerImportSidescanFiles::progressInt, &progress, &QProgressDialog::setValue);
    connect( worker, &WorkerImportSidescanFiles::progressFilename, &progress, &QProgressDialog::setLabelText);

    connect( worker, &WorkerImportSidescanFiles::fileToBeAddedToProjectWindow,
             this, &MainWindow::addFileToProjectWindow);


    workerThread->start();

//    qDebug() << tr( "After 'workerThread->start()'" );

    progress.exec();

    std::string exceptionText = worker->getExceptionString();

    workerThread->quit();
    workerThread->wait();

//    qDebug() << tr( "exceptionText:" ) << QString::fromStdString(exceptionText);


    if ( exceptionText == "" ) {
        updateSelectedFile(lastFile);

        statusBar()->showMessage("Sidescan data loaded");
    }
    else {

        // TODO: ? Close ProgressDialog or does the worker class do it properly?

        //TODO: whine message box

        std::cerr << exceptionText << std::endl;
    }


}

void MainWindow::updateSelectedFile(SidescanFile * newFile){

    selectedFile = newFile;

    tabs->clear(); //TODO: does this leak?

    if(selectedFile){
        /* Update tabs -----------------------------*/
        int n = 0;

        for(auto i= selectedFile->getImages().begin();i!=selectedFile->getImages().end();i++){

            ImageTab* newTab = new ImageTab(*selectedFile,**i,(QWidget*)this);
            newTab->setObjectName( "imageTab with n=" + QString::number( n ) );
            connect(newTab,&ImageTab::inventoryChanged,this,&MainWindow::refreshObjectInventory);

            tabs->addTab(
                        newTab,
                        QString::fromStdString(
                                                (**i).getChannelName()

                                               )
            );
            n++;
        }

        /* Update file info window ------------------- */
        fileInfo->setFile(selectedFile);
    }
    else {
        fileInfo->setFile(NULL);
    }
}


void MainWindow::actionQuit(){
    this->close();
}

void MainWindow::actionAbout(){
    AboutDialog about(this);

    about.exec();
}

void MainWindow::fileSelected(const QItemSelection & selection){

//    qDebug() << tr("MainWindow::fileSelected()");

    if(!selection.indexes().empty()){

//        qDebug() << tr("if(!selection.indexes().empty())");

        if(projectWindow){

//            qDebug() << tr("if(projectWindow)");

            SidescanFile * file = projectWindow->getSelectedFile();

            if(file){

//                qDebug() << tr("if(file)");

                updateSelectedFile(file);
            }
        }
    }
}

void MainWindow::actionFindObjects(){
    if(Project * p = projectWindow->getProject()){
        std::vector<GeoreferencedObject*> newObjects;

        DetectionWindow detectionWindow(p->getFiles(),
                                        fastThresholdValue,
                                        fastTypeValue,
                                        fastNonMaxSuppressionValue,
                                        dbscanEpsilonValue,
                                        dbscanMinPointsValue,
                                        mserDeltaValue,
                                        mserMinimumAreaValue,
                                        mserMaximumAreaValue,
                                        showFeatureMarkersValue,
                                        mergeOverlappingBoundingBoxesValue,
                                        this
        );

        if(detectionWindow.exec() == QDialog::Accepted){
            refreshObjectInventory();
        }
    }
}


void MainWindow::actionSaveObjectImages(){
//    std::cout << "\nMainWindow::actionSaveObjectImages()\n" << std::endl;


    if( ! currentProject )
        return;


    QFileDialog dialog( this,
                         tr( "Save Object Images"),
                        "",
                        tr( "All Files (*)") );

    dialog.setFileMode( QFileDialog::AnyFile );

    dialog.setLabelText( QFileDialog::Accept, tr( "Select" ) ) ; // Name of the button, to replace the default "Open"

    dialog.setViewMode( QFileDialog::Detail );
    dialog.setOptions( QFileDialog::DontConfirmOverwrite );
    dialog.setOption( QFileDialog::DontUseNativeDialog, true );

    QStringList fileNames;

    if ( dialog.exec() )
        fileNames = dialog.selectedFiles();

    if ( fileNames.size() <= 0 )
        return;

    QString file = fileNames.at( 0 );

    QFileInfo fileInfo( file );

    if ( fileInfo.suffix() != "html" ) {
        file += ".html";
        fileInfo.setFile( file );
    }

    // Verify if the file name alread exists
    if ( fileInfo.exists() ) {

        std::string toDisplay = "The file \n\n\"" + file.toStdString() + "\"\n\nalready exists";

        qDebug() << tr( toDisplay.c_str() );

        QMessageBox::warning( this, tr("Warning"), tr( toDisplay.c_str() ), QMessageBox::Ok );

        return;
    }


    // Verify if a folder with the corresponding name (without .html) already exists
    QString folder = fileInfo.absolutePath() + "/" + fileInfo.completeBaseName();

    QFileInfo folderInfo( folder );

    if ( folderInfo.exists() && folderInfo.isDir() )
    {
        std::string toDisplay = "The folder \n\n\"" + folder.toStdString() + "\"\n\nalready exists";

        qDebug() << tr( toDisplay.c_str() );

        QMessageBox::warning( this, tr("Warning"), tr( toDisplay.c_str() ), QMessageBox::Ok );

        return;
    }


    // Try and create the folder in which to put the images
    QDir dir( fileInfo.absolutePath() );

    if ( ! dir.mkdir( fileInfo.completeBaseName() ) )
    {
        std::string toDisplay = "Could not create the folder \n\n\"" + fileInfo.completeBaseName().toStdString()
                + "\"\n\nin the path \"\n\n" + fileInfo.absolutePath().toStdString() + "\"\n";

        qDebug() << tr( toDisplay.c_str() );

        QMessageBox::warning( this, tr("Warning"), tr( toDisplay.c_str() ), QMessageBox::Ok );

        return;
    }

//    std::cout << "\nAfter verifying folder\n" << std::endl;

    currentProject->saveObjectImages( fileInfo.absolutePath(), fileInfo.completeBaseName() );
}


void MainWindow::actionExportTrainingObjectSamples()
{
    std::cout << "\nBeginning MainWindow::actionExportTrainingObjectSamples()\n" << std::endl;

    if( ! currentProject )
        return;


    ParameterscvCreateTrainingSamples parameterscvCreateTrainingSamples;

    QString folder = "/home/chris/Documents/TestAutoSaveTrainingSamples";




//    TrainingSamplesWindow * dialog = new TrainingSamplesWindow( this, folder,
//                                                 parameterscvCreateTrainingSamples);


//    // TODO ? non native

//    dialog->setWindowModality( Qt::WindowModal );

//    dialog->exec();

//    if ( dialog->getUserDidCancel() == false )
//    {
//        // TODO: get parameters and folders

//        currentProject->createAndSaveTrainingObjectSamples( folder, parameterscvCreateTrainingSamples );

//    }



    TrainingSamplesWindow dialog( this, folder,
                                                 parameterscvCreateTrainingSamples);

    // TODO ? non native

    dialog.setWindowModality( Qt::WindowModal );

    dialog.exec();

//    if ( dialog.getUserDidCancel() == false )
//    {
//        qDebug() << "User did not cancel\n";


//        // TODO: get parameters and folders

////        currentProject->createAndSaveTrainingObjectSamples( folder, parameterscvCreateTrainingSamples );

//    }







//    std::cout << "\nEnd of MainWindow::actionExportTrainingObjectSamples()\n" << std::endl;


//    qDebug() <<  "End of MainWindow::actionExportTrainingObjectSamples()\n";




}

void MainWindow::refreshTabs(){
    if(tabs){
        for(unsigned int i=0;i<tabs->count();i++){
            ((ImageTab*)tabs->widget(i))->refreshImage();
        }
    }
}

void MainWindow::objectSelected(GeoreferencedObject * object){
    updateSelectedFile(&object->getFile());
    selectImageTab(object);

    //TODO: scroll to object
}

void MainWindow::selectImageTab(GeoreferencedObject * object){
    for(unsigned int i=0; i<tabs->count();i++){
        ImageTab * tab = (ImageTab*)tabs->widget(i);
        if( tab->getImage() == &object->getImage()){
            //select tab
            tabs->setCurrentIndex(i);

            //scroll tab's image view
            tab->getScrollArea().ensureVisible(object->getX(),object->getY(),50,50);
        }
    }
}


void MainWindow::actionOpen()
{
    if(promptProject()){
        QString fileName = QFileDialog::getOpenFileName(this, 
                                                        tr("Open Sidescan Project"), //tr("Sidescan Project Files"),
														"", 
														tr("Sidescan Project Files (*.ssp)"), 
														nullptr,
														QFileDialog::DontUseNativeDialog );

        if(fileName.size() > 0){

            if(currentProject){
                actionClose();
            }

            std::string sFilename = fileName.toStdString();
            currentProject = new Project();

            QProgressDialog progress("Loading project data...", QString(), 0, 0, this);
            progress.setWindowModality(Qt::WindowModal);
            progress.setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint | Qt::WindowTitleHint);

            progress.setValue(0);
            progress.setMinimumDuration( 0 );

            QThread * workerThread = new QThread( this );

            WorkerOpenProject * worker = new WorkerOpenProject( currentProject, sFilename );

            worker->moveToThread(workerThread);

            connect( workerThread, &QThread::finished, worker, &WorkerOpenProject::deleteLater );
            connect( workerThread, &QThread::started, worker, &WorkerOpenProject::doWork );





            connect( worker, &WorkerOpenProject::done, &progress, &QProgressDialog::cancel);

            workerThread->start();

            progress.exec();

            workerThread->quit();
            workerThread->wait();

            currentProject->setFilename(sFilename);

            refreshProjectUI();

//            projectWindow->displayInfoOnTreeView();


        }
    }
}

void MainWindow::actionSave()
{
    if(currentProject){
        if(currentProject->getFilename().size() > 0){
            currentProject->write(currentProject->getFilename());
            this->setWindowTitle(QString::fromStdString(std::string(currentProject->getFilename())));
        }
        else{
            actionSaveAs();
        }
    }
}

void MainWindow::actionSaveAs(){
    if(currentProject){
        QString fileName = QFileDialog::getSaveFileName( this, 
                                                          tr("Save Sidescan Project"), // tr("Sidescan Project Files"),
														  "", 
														  tr("Sidescan Project Files (*.ssp)"), 
															nullptr,
															QFileDialog::DontUseNativeDialog );

        if(fileName.size() > 0){
            std::string sFilename = fileName.toStdString();

            if(!StringUtils::ends_with(sFilename.c_str(),".ssp")){
                sFilename = sFilename + std::string(".ssp");
            }
            currentProject->write(sFilename);
            currentProject->setFilename(sFilename);
            this->setWindowTitle(QString::fromStdString(std::string(currentProject->getFilename())));
        }
    }
}

void MainWindow::actionExportKmlFile(){
    if(currentProject){
        //QFileDialog::setDefaultSuffix(QString::fromStdString(".kml"));
        QString fileName = QFileDialog::getSaveFileName( this, 
                                                        tr("Export as KML File"), // tr("KML File"),
														"",
														tr("KML File (*.kml)"), 
															nullptr,
															QFileDialog::DontUseNativeDialog );

        if(fileName.size() > 0){
            std::string sFilename = fileName.toStdString();

            if(!StringUtils::ends_with(sFilename.c_str(),".kml")){
                sFilename = sFilename + std::string(".kml");
            }

            currentProject->exportInventoryAsKml(sFilename);
        }
    }
}

void MainWindow::actionClose(){
    if(currentProject){
        delete currentProject;
        currentProject = NULL;
    }
}

void MainWindow::removeSidescanFileFromProject( SidescanFile * file )
{
//    qDebug() << tr("Inside 'MainWindow::removeSidescanFileFromProject()'");


//    // TODO: remove found objects
//    refreshProjectUI();

    // Remove file from project
    if( currentProject ) {

        std::vector<SidescanFile *> & files = currentProject->getFiles();

        auto iter = std::find( files.begin(), files.end(), file );

        if ( iter != files.end() )
        {
            files.erase( iter );
        }

    }


    // TODO: remove found objects
    refreshProjectUI();

//    qDebug() << tr("After refreshProjectUI");

    // If no files left in the project, make sure nothing is displayed
    if( !currentProject || currentProject->getFiles().size() == 0 ) {
        updateSelectedFile(NULL);
    }




}


void MainWindow::addFileToProjectWindow( SidescanFile * file )
{
    projectWindow->addFile(file);
}
