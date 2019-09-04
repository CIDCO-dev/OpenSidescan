#ifndef IMAGETAB_H
#define IMAGETAB_H

#include <QWidget>
#include <QScrollArea>
#include <QLabel>
#include <QPixmap>
#include "opencvhelper.h"

class ImageTab : public QWidget
{
private:
    cv::Mat * originalImage;
    cv::Mat displayedImage;

    QLabel * imageLabel;
    QScrollArea * scrollArea;

    void refreshImage();

    Q_OBJECT
public:
    explicit ImageTab(cv::Mat * image,QWidget *parent = 0);

signals:

private slots:
    void saveImage();
    void findFeatures();
    void cleanImage();
};

#endif // IMAGETAB_H
