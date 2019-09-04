#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDir>
#include <QFileDialog>
#include <QTabWidget>
#include <QProgressDialog>
#include <string>
#include <iostream>
#include "imagetab.h"
#include "aboutdialog.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    tabs(new QTabWidget),
    images(NULL),
    parser(NULL)
{
    ui->setupUi(this);
    buildUI();

    statusBar()->showMessage("Load a sidescan file using File->Open menu");
}

void MainWindow::buildUI(){
    this->setCentralWidget(tabs);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::actionOpen(){
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),QDir::homePath(), tr("Sidescan Files (*.xtf)"));
    std::string sFileName= fileName.toStdString();

    if(fileName.size() > 0){
        try{
            //Cleanup previous workspace ------------------
            statusBar()->showMessage("Loading sidescan data...");
            if(images){
                for(auto i=images->begin();i!= images->end();i++){
                    delete *i;
                }

                tabs->clear(); //Does this leak?

                images->clear();
            }

            if(parser){
                delete parser;
                parser = NULL;
            }

            //Load file -----------------------------------
            QProgressDialog progress("Loading files...", QString(), 0, 2, this);
            progress.setWindowModality(Qt::ApplicationModal);
            progress.show();
            QCoreApplication::processEvents();

            SidescanImager imager;
            parser = DatagramParserFactory::build(sFileName,imager);
            parser->parse(sFileName);
            progress.setValue(1);
            progress.setLabelText(QString("Generating images..."));
            QCoreApplication::processEvents();


            images = imager.generateImages();

            int n = 0;
            for(auto i= images->begin();i!=images->end();i++){

                std::stringstream ss;

                ss << "Channel " << n;

                ImageTab* newTab = new ImageTab(*i);

                tabs->addTab(newTab,ss.str().c_str());
                n++;
            }

            progress.setValue(2);
            progress.reset();
            QCoreApplication::processEvents();

            statusBar()->showMessage("Sidescan data loaded");
        }
        catch(std::exception * e){
            //TODO: whine message box
            std::cerr << e->what() << std::endl;
        }
    }
}



void MainWindow::actionQuit(){
    this->close();
}

void MainWindow::actionAbout(){
    AboutDialog about(this);

    about.exec();
}

