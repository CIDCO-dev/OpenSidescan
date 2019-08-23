#include "imagetab.h"

#include <QLayout>
#include <QToolBar>
#include <QFileDialog>
#include <QDir>
#include <QAction>

ImageTab::ImageTab(cv::Mat * image,QWidget *parent) : QWidget(parent),image(image)
{
    QLayout * layout = new QVBoxLayout();

    //Init toolbar
    QToolBar * toolbar = new QToolBar();
    QAction * saveAction = new QAction(QIcon(":/Images/resources/export.png"),tr("&Export Image"));

    connect(saveAction,&QAction::triggered,this,&ImageTab::saveImage);
    toolbar->addAction(saveAction);

    //Init image scroll area
    QScrollArea * scrollArea = new QScrollArea();
    QLabel * imageLabel = new QLabel();

    imageLabel->setBackgroundRole(QPalette::Base);
    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    imageLabel->setScaledContents(true);

    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(imageLabel);

    QPixmap pixmap = OpencvHelper::cvMatToQPixmap(*image);
    imageLabel->setPixmap(pixmap);
    imageLabel->adjustSize();
    scrollArea->setVisible(true);

    layout->addWidget(toolbar);
    layout->addWidget(scrollArea);

    this->setLayout(layout);
}

void ImageTab::saveImage(){
    QFileDialog saveDialog(this);
    saveDialog.setAcceptMode(QFileDialog::AcceptSave);
    saveDialog.setDirectory(QDir::homePath());
    saveDialog.setDefaultSuffix(".jpg");

    if(saveDialog.exec()){
        QString fileName = saveDialog.selectedFiles().at(0);
        if(fileName.size() > 0){
            cv::imwrite(fileName.toStdString(),*image);
        }

    }
}
