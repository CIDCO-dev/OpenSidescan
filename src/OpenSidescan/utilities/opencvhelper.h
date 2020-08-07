#ifndef OPENCVHELPER_H
#define OPENCVHELPER_H


#include "opencv2/opencv.hpp"
#include "../sidescan/sidescanimage.h"

class OpencvHelper
{
public:
    static bool isInsideImage(double x,double y,cv::Mat & img);

    static void draw(SidescanImage & img,bool showObjectName, bool showObjectBoundingBox,bool showObjectSize,bool showObjectCenter, bool showMicroFeatures);

    static bool isOverlapping(cv::Rect & r1,cv::Rect & r2);

    static void mergeOverlapping(std::vector<cv::Rect> & rects);



private:
    static double fn( const double n,
               const double h360,
               const double saturation01,
               const double value01 );

    static void HSVtoRGB( const double h360,
                   const double saturation01,
                   const double value01,
                   std::vector<unsigned char> & rgb );

    static bool buildColorTable();

    static bool helperVariable;
    static cv::Mat colorTable;

};

#endif // OPENCVHELPER_H
