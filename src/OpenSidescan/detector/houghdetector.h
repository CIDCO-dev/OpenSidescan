#ifndef HOUGHDETECTOR_H
#define HOUGHDETECTOR_H

#include "detector.h"

class HoughDetector : public Detector
{
public:
    HoughDetector();
    ~HoughDetector();
    void detect(SidescanImage & image,std::vector<InventoryObject*> & objectsFound);
};

#endif // HOUGHDETECTOR_H
