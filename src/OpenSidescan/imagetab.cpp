#include "imagetab.h"

#include <QLayout>
#include <QToolBar>
#include <QFileDialog>
#include <QDir>
#include <QAction>

#include "qthelper.h"
#include "detectionwindow.h"



ImageTab::ImageTab(SidescanFile & file,SidescanImage & image,QWidget *parent) : file(file),image(image),QWidget(parent)
{

    QLayout * layout = new QVBoxLayout();

    //Init toolbar
    QToolBar * toolbar = new QToolBar();

    QAction * saveAction = new QAction(QIcon(":/Images/resources/export.png"),tr("&Export Image"));
    connect(saveAction,&QAction::triggered,this,&ImageTab::saveImage);
    toolbar->addAction(saveAction);

    toolbar->addSeparator();

    showObjectBoundingBox = new QAction(QIcon(":/Images/resources/bounding-box.png"),tr("&Show bounding boxes"));
    showObjectBoundingBox->setCheckable(true);
    showObjectBoundingBox->setChecked(true);
    connect(showObjectBoundingBox,&QAction::triggered,this,&ImageTab::refreshImage);
    toolbar->addAction(showObjectBoundingBox);

    showObjectSizesAction = new QAction(QIcon(":/Images/resources/ruler.png"),tr("&Show sizes"));
    showObjectSizesAction->setCheckable(true);
    showObjectSizesAction->setChecked(true);
    connect(showObjectSizesAction,&QAction::triggered,this,&ImageTab::refreshImage);
    toolbar->addAction(showObjectSizesAction);

    showObjectCenter = new QAction(QIcon(":/Images/resources/aim.png"),tr("&Show Objects' Center"));
    showObjectCenter->setCheckable(true);
    showObjectCenter->setChecked(false);
    connect(showObjectCenter,&QAction::triggered,this,&ImageTab::refreshImage);
    toolbar->addAction(showObjectCenter);

    //Init image scroll area
    scrollArea = new QScrollArea();
    imageLabel = new ImageTabLabel(*this,file,image);
    connect(imageLabel,&ImageTabLabel::inventoryChanged,this,&ImageTab::refreshInventory);

    imageLabel->setBackgroundRole(QPalette::Base);
    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    imageLabel->setScaledContents(true);

    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(imageLabel);

    refreshImage();

    layout->addWidget(toolbar);
    layout->addWidget(scrollArea);

    this->setLayout(layout);
}

void ImageTab::refreshImage(){
    OpencvHelper::draw(image,showObjectBoundingBox->isChecked(),showObjectSizesAction->isChecked(),showObjectCenter->isChecked());

    QPixmap pixmap = QtHelper::cvMatToQPixmap(image.getDisplayedImage());
    imageLabel->setPixmap(pixmap);
    imageLabel->adjustSize();
    scrollArea->setVisible(true);
}

void ImageTab::saveImage(){
    QFileDialog saveDialog(this);
    saveDialog.setAcceptMode(QFileDialog::AcceptSave);
    saveDialog.setDirectory(QDir::homePath());
    saveDialog.setDefaultSuffix(".jpg");

    if(saveDialog.exec()){
        QString fileName = saveDialog.selectedFiles().at(0);
        if(fileName.size() > 0){
            cv::imwrite(fileName.toStdString(),image.getImage());
        }
    }
}

void ImageTab::refreshInventory(){
    emit inventoryChanged();
    refreshImage();
}

