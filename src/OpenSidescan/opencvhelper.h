#ifndef OPENCVHELPER_H
#define OPENCVHELPER_H

#include <QImage>
#include <QPixmap>
#include "opencv2/opencv.hpp"

class OpencvHelper
{
public:
    static QPixmap cvMatToQPixmap( const cv::Mat &inMat );
    static QImage  cvMatToQImage( const cv::Mat &inMat );
};

#endif // OPENCVHELPER_H
