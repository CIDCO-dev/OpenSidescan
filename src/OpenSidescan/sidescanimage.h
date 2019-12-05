#ifndef SIDESCANIMAGE_H
#define SIDESCANIMAGE_H

#include <opencv2/opencv.hpp>
#include "../thirdParty/MBES-lib/src/sidescan/SidescanPing.hpp"

#include <vector>
#include "georeferencedobject.h"

//forwards
class GeoreferencedObject;




class SidescanImage
{
public:
    //SidescanImage();
    SidescanImage(cv::Mat * m,std::vector<SidescanPing * > * rawData,unsigned int channelNumber,std::string & channelName, std::map<std::string,std::string>  * properties);

    ~SidescanImage();

    cv::Mat & getImage(){ return *image; }
    cv::Mat & getDisplayedImage() { return displayedImage;}

    unsigned int getChannelNumber(){ return channelNumber;}

    std::string & getChannelName(){ return channelName;}

    std::vector<SidescanPing*>  & getPings(){ return *rawData;}

    std::map<std::string,std::string> & getProperties(){ return *properties;}

    std::vector<GeoreferencedObject*> & getObjects(){ return objects;}

    void resetDisplayedImage(){
        if(!displayedImage.empty()){
            displayedImage.release();
            displayedImage = image->clone();
        }
    }

private:
    cv::Mat * image = NULL;

    cv::Mat displayedImage;
    std::vector<SidescanPing * > * rawData = NULL;

    //channel info
    std::map<std::string,std::string>  * properties;
    unsigned int channelNumber;
    std::string channelName;

    //object detection shit
    std::vector<GeoreferencedObject *> objects;
};

#endif // SIDESCANIMAGE_H
