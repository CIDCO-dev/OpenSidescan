#ifndef ROIDETECTOR_H
#define ROIDETECTOR_H

#include "detector.h"
#include "../utilities/opencvhelper.h"
#include "opencv2/opencv.hpp"
#include "../machinelearning/dbscan.h"

class RoiDetector : public Detector
{
public:
    RoiDetector(
            int fastThreshold,
            cv::FastFeatureDetector::DetectorType fastType,
            bool fastNonMaxSuppression,
            double dbscanEpsilon,
            int dbscanMinimumPoints,
            int mserDelta,
            int mserMinimumArea,
            int mserMaximumArea,
            bool mergeOverlappingObjects
            );
   static cv::FastFeatureDetector::DetectorType toCvFastType (int fasType);
    void detect(SidescanImage & image,std::vector<InventoryObject*> & objectsFound);

private:
    int fastThreshold;
    cv::FastFeatureDetector::DetectorType fastType;
    bool fastNonMaxSuppression;
    double dbscanEpsilon;
    int dbscanMinimumPoints;
    int mserDelta;
    int mserMinimumArea;
    int mserMaximumArea;
    bool mergeOverlappingObjects;
};

#endif // ROIDETECTOR_H
