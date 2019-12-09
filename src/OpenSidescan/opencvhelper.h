#ifndef OPENCVHELPER_H
#define OPENCVHELPER_H


#include "opencv2/opencv.hpp"
#include "dbscan.h"
#include "sidescanfile.h"
#include "sidescanimage.h"
#include "georeferencedobject.h"

class OpencvHelper
{
public:
    static void detectObjects(std::vector<GeoreferencedObject*> & objects,SidescanFile & file,SidescanImage & image,int fastThreshold=100,int fastType=cv::FastFeatureDetector::TYPE_9_16,bool fastNonMaxSuppression=false,double dbscanEpsilon=50.0,int dbscanMinimumPoints=10,int mserDelta=5,int mserMinimumArea=60,int mserMaximumArea=14400,bool showMarkers=true,bool mergeOverlappingObjects=true);
    static bool isInsideImage(double x,double y,cv::Mat & img);

    static void draw(SidescanImage & img, bool showObjectBoundingBox,bool showObjectSize,bool showObjectCenter);

    static bool isOverlapping(cv::Rect & r1,cv::Rect & r2);

    static void mergeOverlapping(std::vector<cv::Rect> & rects);
};

#endif // OPENCVHELPER_H
