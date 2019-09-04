#include "imagetab.h"

#include <QLayout>
#include <QToolBar>
#include <QFileDialog>
#include <QDir>
#include <QAction>

ImageTab::ImageTab(cv::Mat * image,QWidget *parent) : QWidget(parent),originalImage(image)
{
    QLayout * layout = new QVBoxLayout();

    //Init toolbar
    QToolBar * toolbar = new QToolBar();
    QAction * saveAction = new QAction(QIcon(":/Images/resources/export.png"),tr("&Export Image"));

    connect(saveAction,&QAction::triggered,this,&ImageTab::saveImage);
    toolbar->addAction(saveAction);

    QAction * findFeaturesAction = new QAction(QIcon(":/Images/resources/marker.png"),tr("&Find Features"));

    connect(findFeaturesAction,&QAction::triggered,this,&ImageTab::findFeatures);
    //toolbar->addAction(findFeaturesAction);

    QAction * cleanImageAction = new QAction(QIcon(":/Images/resources/clear.png"),tr("&Clean"));

    connect(cleanImageAction,&QAction::triggered,this,&ImageTab::cleanImage);
    //toolbar->addAction(cleanImageAction);

    //Init image scroll area
    scrollArea = new QScrollArea();
    imageLabel = new QLabel();

    imageLabel->setBackgroundRole(QPalette::Base);
    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    imageLabel->setScaledContents(true);

    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(imageLabel);

    displayedImage = originalImage->clone();

    refreshImage();

    layout->addWidget(toolbar);
    layout->addWidget(scrollArea);

    this->setLayout(layout);
}

void ImageTab::refreshImage(){
    QPixmap pixmap = OpencvHelper::cvMatToQPixmap(displayedImage);
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
            cv::imwrite(fileName.toStdString(),*originalImage);
        }

    }
}

void ImageTab::findFeatures(){
    std::vector<cv::KeyPoint> fastFeatures;
    std::vector<cv::KeyPoint> mserFeatures;
    //std::vector<cv::KeyPoint> orbFeatures;

    /*Sobel edge detection
    cv::Mat sobel_x,sobel_y;
    cv::Sobel(*originalImage,sobel_x,CV_8UC1,1,0,5);
    cv::Sobel(*originalImage,sobel_y,CV_8UC1,0,1,5);
    cv::addWeighted(sobel_x,0.5,sobel_y,0.5,0,displayedImage);
    cv::Laplacian(displayedImage,displayedImage,CV_8UC1);
    */

    /* FAST feature detection, [Rosten06] */
    cv::FAST(*originalImage,fastFeatures,100,false,cv::FastFeatureDetector::TYPE_9_16);

    cv::Ptr<cv::MSER> detector= cv::MSER::create(5,500,14400,0.5,.2,200,1.01,0.003,5);
    detector->detect(*originalImage,mserFeatures);


    //cv::Ptr<cv::ORB> orbDetector= cv::ORB::create(500,1.2f,8,31,0,2,cv::ORB::HARRIS_SCORE,31,20);
    //orbDetector->detect(*originalImage,orbFeatures);

    /* Blob detection
    cv::SimpleBlobDetector::Params params;
    params.minThreshold=0;
    params.maxThreshold=20;
    params.minDistBetweenBlobs = 50.0f;
    params.filterByInertia = false;
    params.filterByConvexity = false;
    params.filterByColor = true;
    params.blobColor=0;
    params.filterByCircularity = false;
    params.filterByArea = true;
    params.minArea = 1000.0f;
    params.maxArea = 10000.0f;

    cv::Ptr<cv::SimpleBlobDetector> detector = cv::SimpleBlobDetector::create(params);


    detector->detect(displayedImage,features);
    */

    cv::drawKeypoints(displayedImage,fastFeatures,displayedImage,cv::Scalar(0,0,255),cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
    cv::drawKeypoints(displayedImage,mserFeatures,displayedImage,cv::Scalar(0,255,0),cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
    //cv::drawKeypoints(displayedImage,orbFeatures ,displayedImage,cv::Scalar(255,0,0),cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

    refreshImage();
}

void ImageTab::cleanImage(){
    displayedImage= originalImage->clone();
    refreshImage();
}
