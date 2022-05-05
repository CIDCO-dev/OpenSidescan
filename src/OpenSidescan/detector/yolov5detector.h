#ifndef YOLOV5DETECTOR_H
#define YOLOV5DETECTOR_H

#include "detector.h"
//#include "../utilities/opencvhelper.h"
#include "opencv2/opencv.hpp"
#include <opencv2/dnn.hpp>


class Yolov5Detector : public Detector
{
public:
    //Yolov5Detector(std::string modelPath);
    Yolov5Detector(std::string modelPath, float scoresThreshold = 0.5, float nmsThreshold = 0.45, float confidenceThreshold = 0.3);
    void detect(SidescanImage & image, std::vector<InventoryObject*> & objectsFound);

private:

    bool mergeOverlappingObjects;
    cv::dnn::Net net;
    float scoresThreshold;
    float nmsThreshold;
    float confidenceThreshold;
};



#endif // YOLOV5DETECTOR_H
