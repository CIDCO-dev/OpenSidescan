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
#include "ui/mainimage/imagetab.h"
#include "ui/about/aboutdialog.h"
#include "ui/docks/projectwindow/projectwindow.h"
#include "ui/detection/detectionwindow.h"
#include "ui/detection/monitorwindow.h"
#include "ui/docks/inventorywindow.h"
#include "ui/docks/vesselgeometrywindow.h"

#include "workerimportsidescanfiles.h"
#include "workeropenproject.h"

#include "ui/training/workertrainingsamples.h"
#include "ui/training/trainingsampleswindow.h"

#include "refactorme/progressdialognotclosingrightawayoncancel.h"

#include "../../src/thirdParty/MBES-lib/src/utils/StringUtils.hpp"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    tabs(new QTabWidget),
    currentProject(nullptr),
    fileInfo(nullptr),
    folderCreateTrainingSamples( "" )
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

    vesselWindow = new VesselGeometryWindow(this);
    this->addDockWidget(Qt::LeftDockWidgetArea,vesselWindow);
    connect(ui->actionShowVesselGeometryWindow,&QAction::triggered,vesselWindow,&VesselGeometryWindow::show);

    fileInfo = new FilePropertiesWindow(this);
    this->addDockWidget(Qt::RightDockWidgetArea,fileInfo);
    connect(ui->actionShowFilePropertiesWindow,&QAction::triggered,fileInfo,&FilePropertiesWindow::show);

    inventoryWindow = new InventoryWindow(this);
    this->addDockWidget(Qt::BottomDockWidgetArea,inventoryWindow);
    connect(ui->actionShowObjectInventoryWindow,&QAction::triggered,inventoryWindow,&InventoryWindow::show);
    connect(inventoryWindow,&InventoryWindow::objectSelected,this,&MainWindow::objectSelected);

    channelInfo = new ChannelPropertiesWindow(this);
    this->addDockWidget(Qt::RightDockWidgetArea,channelInfo);
    connect(ui->actionShowChannelPropertiesWindow,&QAction::triggered,channelInfo,&ChannelPropertiesWindow::show);
    connect(tabs,&QTabWidget::currentChanged,this,&MainWindow::tabChanged);

    monitorAction = new QAction(QIcon(":/Images/resources/play.png"), QString("Monitor Dir"), this);
    monitorAction->setCheckable(true);
    this->ui->mainToolBar->addAction(monitorAction);
    connect(monitorAction, SIGNAL(triggered()), this, SLOT(monitorActionTriggered()));


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
        QMessageBox msgBox( this );
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

    vesselWindow->setProject(currentProject);

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
    selectedImage = NULL;

    tabs->clear(); //TODO: does this leak?

    if(selectedFile){
        /* Update tabs -----------------------------*/
        int n = 0;

        for(auto i= selectedFile->getImages().begin();i!=selectedFile->getImages().end();i++){

            if(!selectedImage){
                selectedImage = &(**i);
            }

            ImageTab* newTab = new ImageTab(**i,(QWidget*)this);
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
    }

    fileInfo->updateModel(selectedFile);
    channelInfo->updateModel(selectedImage );
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

        if(p->getFileCount() > 0){

            DetectionWindow detectionWindow(*p,
                                            fastThresholdValue,
                                            fastTypeValue,
                                            fastNonMaxSuppressionValue,
                                            dbscanEpsilonValue,
                                            dbscanMinPointsValue,
                                            mserDeltaValue,
                                            mserMinimumAreaValue,
                                            mserMaximumAreaValue,
                                            mergeOverlappingBoundingBoxesValue,
                                            this
            );

            if(detectionWindow.exec() == QDialog::Accepted){
                refreshObjectInventory();
            }
        }
        else{
            QMessageBox::critical(this, tr("Error"),tr("No files found in project. Please import some files first."),QMessageBox::Ok);
        }
    }
}


void MainWindow::actionExportHTML(){
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
                + "\"\n\nin the path\n\n\"" + fileInfo.absolutePath().toStdString() + "\"\n";

        qDebug() << tr( toDisplay.c_str() );

        QMessageBox::warning( this, tr("Warning"), tr( toDisplay.c_str() ), QMessageBox::Ok );

        return;
    }

    currentProject->saveObjectImages( fileInfo.absolutePath(), fileInfo.completeBaseName() );
}


void MainWindow::actionExportTrainingObjectSamples()
{
//    std::cout << "\nBeginning MainWindow::actionExportTrainingObjectSamples()\n" << std::endl;

    if( ! currentProject ) {

        std::string toDisplay = "There is no active project.\n";

        qDebug() << tr( toDisplay.c_str() );

        QMessageBox::warning( this, tr("Warning"), tr( toDisplay.c_str() ), QMessageBox::Ok );
        return;
    }



    if ( currentProject->getFileCount() == 0 ) {

        std::string toDisplay = "There are no sidescan files.\n";

        qDebug() << tr( toDisplay.c_str() );

        QMessageBox::warning( this, tr("Warning"), tr( toDisplay.c_str() ), QMessageBox::Ok );
        return;
    }


    if ( currentProject->getObjectCount() == 0 ) {

        std::string toDisplay = "There are no objects.\n";

        qDebug() << tr( toDisplay.c_str() );

        QMessageBox::warning( this, tr("Warning"), tr( toDisplay.c_str() ), QMessageBox::Ok );
        return;
    }




    TrainingSamplesWindow dialog( this, folderCreateTrainingSamples,
                          parameterscvCreateTrainingSamples);

    // TODO ? non native

    dialog.setWindowModality( Qt::WindowModal );

    dialog.exec();

    if ( dialog.getUserDidCancel() == false )
    {
        //qDebug() << "User did not cancel\n";

        dialog.getFolder( folderCreateTrainingSamples );
        dialog.getParameters( parameterscvCreateTrainingSamples );

        createAndSaveTrainingObjectSamples( folderCreateTrainingSamples,
                                 parameterscvCreateTrainingSamples );
    }


//    std::cout << "\nEnd of MainWindow::actionExportTrainingObjectSamples()\n" << std::endl;

//    qDebug() <<  "End of MainWindow::actionExportTrainingObjectSamples()\n";

}


void MainWindow::createAndSaveTrainingObjectSamples( const QString & folder,
                    const ParameterscvCreateTrainingSamples & parameters )
{
    unsigned int numberOfObjects = currentProject->getObjectCount();

    if ( numberOfObjects == 0 )
        return;

    std::string originalObjectImages = "original";
    std::string outputPositiveSamples = "positive";
    std::string background = "background";

    QString folderOriginalObjectImages = folder + "/" + QObject::tr( originalObjectImages.c_str() );
    QString folderOutputPositiveSamples = folder + "/" + QObject::tr( outputPositiveSamples.c_str() );
    QString folderBackground = folder + "/" + QObject::tr( background.c_str() );


    // Open bg.txt file

    QString fileNameBgDotTxt = folderBackground + "/" + "bg.txt";

    std::ofstream outFile;
    outFile.open( fileNameBgDotTxt.toStdString(), std::ofstream::out );

    if ( outFile.is_open() == false ) {

        std::string toDisplay = "Cannot create file\n\n\""
                                    + fileNameBgDotTxt.toStdString() + "\"\n";

        qDebug() << tr( toDisplay.c_str() );

        QMessageBox::warning( this, tr("Warning"), tr( toDisplay.c_str() ), QMessageBox::Ok );
        return;
    }


    BoolWithMutex continueToCreateAndSaveTrainingObjectSamples( true );


    ProgressDialogNotClosingRightAwayOnCancel progress(
                            "Creating and saving training object samples...",
                            "Stop", "Stopping...",
                             0, numberOfObjects + 2,
                            &continueToCreateAndSaveTrainingObjectSamples,
                            true, this );
    // TODO ? non native

    progress.setWindowModality(Qt::WindowModal);
    progress.setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint | Qt::WindowTitleHint);

    QThread * workerThread = new QThread( this );

    WorkerTrainingSamples * worker = new WorkerTrainingSamples( currentProject,
                                           numberOfObjects,
                                            parameters,
                                            folderOriginalObjectImages,
                                            folderOutputPositiveSamples,
                                            folderBackground,
                                            outFile,
                                            &continueToCreateAndSaveTrainingObjectSamples );

    worker->moveToThread(workerThread);

    connect( workerThread, &QThread::finished, worker, &WorkerTrainingSamples::deleteLater );
    connect( workerThread, &QThread::started, worker, &WorkerTrainingSamples::doWork );

    connect( worker, &WorkerTrainingSamples::progress,
             &progress, &ProgressDialogNotClosingRightAwayOnCancel::setValue);

    connect( worker, &WorkerTrainingSamples::done,
             &progress, &ProgressDialogNotClosingRightAwayOnCancel::closeDialog );

    workerThread->start();

    progress.exec();

    workerThread->quit();
    workerThread->wait();
}



void MainWindow::refreshTabs(){
    if(tabs){
        for(unsigned int i=0;i<tabs->count();i++){
            ((ImageTab*)tabs->widget(i))->refreshImage();
        }
    }
}

void MainWindow::objectSelected(InventoryObject * object){


    projectWindow->selectFile( & object->getImage().getFile() );


//    updateSelectedFile(&object->getFile());

    selectImageTab(object);

    //TODO: scroll to object
}

void MainWindow::selectImageTab(InventoryObject * object){
    for(unsigned int i=0; i<tabs->count();i++){
        ImageTab * tab = (ImageTab*)tabs->widget(i);
        if( tab->getImage() == &object->getImage()){
            //select tab
            tabs->setCurrentIndex(i);

            selectedImage = tab->getImage();
            channelInfo->updateModel(selectedImage);

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

            projectWindow->selectLastFile();

//            if ( model->getNbFiles() > 0 )
//                tree->setCurrentIndex( model->getModelIndexFileIndex( model->getNbFiles() - 1 ) );


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

void MainWindow::actionExportCsvFile(){

    if(currentProject){
        QString fileName = QFileDialog::getSaveFileName( this,
                                                        tr("Export as CSV File"),
                                                        "",
                                                        tr("CSV File (*.csv)"),
                                                            nullptr,
                                                            QFileDialog::DontUseNativeDialog );

        if(fileName.size() > 0){
            std::string sFilename = fileName.toStdString();

            if(!StringUtils::ends_with(sFilename.c_str(),".csv")){
                sFilename = sFilename + std::string(".csv");
            }

            currentProject->exportInventoryAsCsv(sFilename);
        }
    }

}




void MainWindow::actionClose(){
    if(currentProject){
        delete currentProject;
        currentProject = NULL;
        selectedFile = NULL;
        selectedImage = NULL;
    }
}

void MainWindow::removeSidescanFileFromProject( SidescanFile * file )
{
//    qDebug() << tr("Inside 'MainWindow::removeSidescanFileFromProject()'");


//    // TODO: remove found objects
//    refreshProjectUI();

    // Remove file from project
    if( currentProject ) {

        currentProject->removeFile(file);

    }

//    // TODO: remove found objects
    refreshProjectUI();

//    inventoryWindow->setProject(currentProject);


//    qDebug() << tr("After refreshProjectUI");

    // If no files left in the project, make sure nothing is displayed
    if( !currentProject ) {
        updateSelectedFile(NULL);
    }
    else {
        if ( currentProject->getFileCount() == 0 ) {
            updateSelectedFile(NULL);
        } else {
            projectWindow->selectLastFile();
        }
    }
}


void MainWindow::addFileToProjectWindow( SidescanFile * file )
{
    projectWindow->addFile(file);
    refreshObjectInventory();
}

void MainWindow::tabChanged( int index )
{
        if( selectedFile && tabs && tabs->count() > 0 && index >= 0 && index < tabs->count() ){
            selectedImage = ( (ImageTab*)tabs->widget(index) )->getImage();
            channelInfo->updateModel( selectedImage );
        }
        else{
            selectedImage = NULL;
            channelInfo->updateModel( selectedImage );
        }
}

void MainWindow::monitorProgress(QString progress)
{
    ui->statusBar->showMessage(progress);
}

void MainWindow::monitorActionTriggered() {
    if(!monitorAction->isChecked()) {

        ui->statusBar->showMessage(QString::fromStdString("Stopping..."));

        if(monitorThread){
            monitorThread->stop();
        }

        monitorAction->setIcon(QIcon(":/Images/resources/play.png"));
        monitorAction->setChecked(false);
    } else {

        QFileDialog folderDiag(this,  tr("Select folder to monitor"), "");
        folderDiag.setFileMode(QFileDialog::Directory);
        folderDiag.setOptions(QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

        if( ! folderDiag.exec() ) {
             monitorAction->setChecked(false);
        } else {
            QString dir = folderDiag.selectedFiles().at(0);

            MonitorWindow monitorWindow(*currentProject,
                                            fastThresholdValue,
                                            fastTypeValue,
                                            fastNonMaxSuppressionValue,
                                            dbscanEpsilonValue,
                                            dbscanMinPointsValue,
                                            mserDeltaValue,
                                            mserMinimumAreaValue,
                                            mserMaximumAreaValue,
                                            mergeOverlappingBoundingBoxesValue,
                                            this);

            if(monitorWindow.exec() == QDialog::Accepted){
                //refreshObjectInventory();
                ui->statusBar->showMessage(QString::fromStdString("Monitoring directory for new sidescan files..."));

                if(monitorThread){
                    delete monitorThread;
                    monitorThread = nullptr;
                }

                monitorThread = new MonitorThread(dir, currentProject, monitorWindow.getDetector());
                connect( monitorThread, &MonitorThread::fileToBeAddedToProjectWindow,
                         this, &MainWindow::addFileToProjectWindow);
                connect(monitorThread, &MonitorThread::updateProgress, this, &MainWindow::monitorProgress);

                monitorAction->setIcon(QIcon(":/Images/resources/stop.png"));
                monitorThread->start();
            } else {
                //reset to "play" icon since action was canceled
                monitorAction->setIcon(QIcon(":/Images/resources/play.png"));
                monitorAction->setChecked(false);
            }
        }
    }
}

void MainWindow::actionExportHitsFile(){
    if(currentProject){
        QString path = QFileDialog::getExistingDirectory(this,
                                                         tr("Select Directory"),
                                                         "",
                                                         QFileDialog::ShowDirsOnly
                                                         | QFileDialog::DontResolveSymlinks);

        if(path.size() > 0){
            std::string sPath = path.toStdString();


            currentProject->exportInventoryAsHits(sPath);
        }
    }
}

void MainWindow::actionExportPyTorch(){
    if(currentProject){
        QString fileName = QFileDialog::getSaveFileName( this,
                                                        tr("Export as PyHits"),
                                                        "",
                                                        tr("CSV File (*.PyHits)"),
                                                            nullptr,
                                                            QFileDialog::DontUseNativeDialog );

        if(fileName.size() > 0){
            std::string sFilename = fileName.toStdString();

            if(!StringUtils::ends_with(sFilename.c_str(),".PyHits")){
                sFilename = sFilename + std::string(".PyHits");
            }

            currentProject->exportInventory4PyTorch(sFilename);
        }
    }

}
