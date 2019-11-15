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
#include <iostream>
#include "imagetab.h"
#include "aboutdialog.h"
#include "projectwindow.h"
#include "detectionwindow.h"
#include "inventorywindow.h"

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
            delete currentProject;
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
        msgBox.setInformativeText("All unsaved changed will be lost.");
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

    //Set window title
    std::string title = currentProject->getFilename().size() > 0 ? currentProject->getFilename() : "New Project";
    this->setWindowTitle(QString::fromStdString(title));
}

void MainWindow::actionImport(){
    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Import Sidescan Files"),QDir::homePath(), tr("Sidescan Files (*.xtf)"));

    if(fileNames.size() > 0){

        try{
            statusBar()->showMessage("Loading sidescan data...");

            //Load file -----------------------------------*
            unsigned int nbFiles = fileNames.size();

            QProgressDialog progress("Loading files...", QString(), 0, nbFiles*2, this);
            progress.setWindowModality(Qt::ApplicationModal);
            progress.setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint | Qt::WindowTitleHint);
            progress.show();
            QCoreApplication::processEvents();

            SidescanFile * lastFile = NULL; //used to "open" a file after the import loop

            unsigned int operationCount=0;

            for(auto i=fileNames.begin();i!=fileNames.end();i++){
                std::string sFileName = (*i).toStdString();

                if(!projectWindow->containsFile(sFileName)){
                    SidescanImager imager;

                    DatagramParser * parser = DatagramParserFactory::build(sFileName,imager);
                    parser->parse(sFileName);

                    progress.setValue(operationCount++);
                    progress.setLabelText(QString::fromStdString(sFileName));
                    QCoreApplication::processEvents();

                    SidescanFile * file = imager.generate(sFileName);

                    projectWindow->addFile(file);

                    lastFile = file;

                    progress.setValue(operationCount++);
                    QCoreApplication::processEvents();
                    delete parser;
                    parser=NULL;
                }
                else{
                    //Project already contains file. Ignore silently.
                }
            }

            progress.reset();
            updateSelectedFile(lastFile);

            statusBar()->showMessage("Sidescan data loaded");
        }
        catch(std::exception * e){
            //TODO: whine message box
            std::cerr << e->what() << std::endl;
        }
    }
}

void MainWindow::updateSelectedFile(SidescanFile * newFile){

    selectedFile = newFile;

    if(selectedFile){
        /* Update tabs -----------------------------*/
        int n = 0;

        tabs->clear(); //TODO: does this leak?

        for(auto i= selectedFile->getImages().begin();i!=selectedFile->getImages().end();i++){

            ImageTab* newTab = new ImageTab(*selectedFile,**i,(QWidget*)this);
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
}


void MainWindow::actionQuit(){
    this->close();
}

void MainWindow::actionAbout(){
    AboutDialog about(this);

    about.exec();
}

void MainWindow::fileSelected(const QItemSelection & selection){
    if(!selection.indexes().empty()){
        if(projectWindow){
            SidescanFile * file = projectWindow->getSelectedFile();
            if(file){
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
        QString fileName = QFileDialog::getOpenFileName(this, tr("Sidescan Project Files"),QDir::homePath(), tr("Sidescan Project Files (*.ssp)"));

        if(fileName.size() > 0){
            if(currentProject){
                delete currentProject;
            }

            std::string sFilename = fileName.toStdString();
            currentProject = new Project();
            currentProject->read(sFilename);
            currentProject->setFilename(sFilename);

            refreshProjectUI();
        }
    }
}

void MainWindow::actionSave()
{
    if(currentProject){
        if(currentProject->getFilename().size() > 0){
            currentProject->write(currentProject->getFilename());
            refreshProjectUI();
        }
        else{
            actionSaveAs();
        }
    }
}

void MainWindow::actionSaveAs(){
    if(currentProject){
        QString fileName = QFileDialog::getSaveFileName(this, tr("Sidescan Project Files"),QDir::homePath(), tr("Sidescan Project Files (*.ssp)"));

        if(fileName.size() > 0){
            std::string sFilename = fileName.toStdString();
            currentProject->write(sFilename);
            currentProject->setFilename(sFilename);
            refreshProjectUI();
        }
    }
}

void MainWindow::actionExportKmlFile(){
    if(currentProject){
        //QFileDialog::setDefaultSuffix(QString::fromStdString(".kml"));
        QString fileName = QFileDialog::getSaveFileName(this, tr("KML File"),QDir::homePath(), tr("KML File (*.kml)"));

        if(fileName.size() > 0){
            std::string sFilename = fileName.toStdString();
            currentProject->exportInventoryAsKml(sFilename);
        }
    }
}
