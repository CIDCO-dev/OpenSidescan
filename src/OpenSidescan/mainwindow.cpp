#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDir>
#include <QFileDialog>
#include <QTabWidget>
#include <QProgressDialog>
#include <QDockWidget>
#include <string>
#include <QVBoxLayout>
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




    createProject();

    //center
    this->setCentralWidget(tabs);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete projectWindow;
}

void MainWindow::createProject(){
    //TODO: handle previous project's cleanup and closure

    currentProject = new Project();

    projectWindow->setProject(currentProject);
    inventoryWindow->setProject(currentProject);

}

void MainWindow::actionOpen(){
    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Open File"),QDir::homePath(), tr("Sidescan Files (*.xtf)"));
    //std::string sFileName= fileName.toStdString();

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
                SidescanImager imager;
                std::string sFileName = (*i).toStdString();
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

    /* Update tabs -----------------------------*/
    int n = 0;

    tabs->clear(); //TODO: does this leak?

    for(auto i= selectedFile->getImages().begin();i!=selectedFile->getImages().end();i++){

        ImageTab* newTab = new ImageTab(*selectedFile,**i,(QWidget*)this);

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
                                        mserMaximumVariationValue,
                                        mserMinimumDiversityValue,
                                        mserMaximumEvolutionValue,
                                        mserAreaThresholdValue,
                                        mserMinimumMarginValue,
                                        mserEdgeBlurValue,
                                        showFeatureMarkersValue,
                                        mergeOverlappingBoundingBoxesValue,
                                        this
        );

        if(detectionWindow.exec() == QDialog::Accepted){
            inventoryWindow->refreshInventoryTable();
            refreshTabs();
        }
    }
}

void MainWindow::actionCleanImage(){
    //TODO
    //displayedImage= image.getImage().clone();
    //refreshImage();
}

void MainWindow::refreshTabs(){
    if(tabs){
        for(unsigned int i=0;i<tabs->count();i++){
            ((ImageTab*)tabs->widget(i))->refreshImage();
        }
    }
}

void MainWindow::refreshObjectInventory(){
    if(inventoryWindow){
        inventoryWindow->refreshInventoryTable();
    }
}


