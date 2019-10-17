#ifndef QTHELPER_H
#define QTHELPER_H

#include <QImage>
#include <QPixmap>
#include <opencv2/opencv.hpp>

class QtHelper
{
public:
    QtHelper();

    static QPixmap cvMatToQPixmap( const cv::Mat &inMat );
    static QImage  cvMatToQImage( const cv::Mat &inMat );
};

#endif // QTHELPER_H
