#ifndef DETECTOR_H
#define DETECTOR_H

#include <vector>
#include "../inventoryobject/inventoryobject.h"


class Detector
{
public:
    Detector();
    virtual ~Detector();

    virtual void detect(SidescanImage & image,std::vector<InventoryObject*> & objectsFound) = 0;
};

#endif // DETECTOR_H
