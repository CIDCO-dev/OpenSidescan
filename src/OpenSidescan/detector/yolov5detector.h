#ifndef YOLOV5DETECTOR_H
#define YOLOV5DETECTOR_H

#include "detector.h"
//#include "../utilities/opencvhelper.h"
#include "opencv2/opencv.hpp"
#include <opencv2/dnn.hpp>


class Yolov5Detector : public Detector
{
public:
    Yolov5Detector();
    void detect(SidescanImage & image,std::vector<InventoryObject*> & objectsFound);

private:

    bool mergeOverlappingObjects;
    cv::dnn::Net net;
};



#endif // YOLOV5DETECTOR_H
