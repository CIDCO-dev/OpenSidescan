#ifndef SIDESCANIMAGE_H
#define SIDESCANIMAGE_H

#include <opencv2/opencv.hpp>
#include "../thirdParty/MBES-lib/src/sidescan/SidescanPing.hpp"

#include <vector>
#include "inventoryobject/inventoryobject.h"

//forwards
class InventoryObject;
class SidescanFile;



class SidescanImage
{
public:
    //SidescanImage();
    SidescanImage(SidescanFile & file,cv::Mat * m,std::vector<SidescanPing * > * rawData,unsigned int channelNumber,std::string & channelName, unsigned int channelType, std::map<std::string,std::string>  * properties);

    ~SidescanImage();

    SidescanFile & getFile() {return file;}

    cv::Mat & getImage(){ return *image; }
    cv::Mat & getDisplayedImage() { return displayedImage;}

    unsigned int getChannelNumber(){ return channelNumber;}

    unsigned int getChannelType(){ return channelType;}

    std::string & getChannelName(){ return channelName;}

    //XTF documentation: port channel type is 1
    bool isPort() {return channelType==1;}

    //XTF documentation: starboard channel type is 2
    bool isStarboard() {return channelType==2;};

    std::vector<SidescanPing*>  & getPings(){ return *rawData;}

    std::map<std::string,std::string> & getProperties(){ return *properties;}

    std::vector<InventoryObject*> & getObjects(){ return objects;}

    std::vector<cv::KeyPoint> & getMicroFeatures(){ return microFeatures;}

    void setMicroFeatures(std::vector<cv::KeyPoint> microFeatures) {this->microFeatures = microFeatures;}

    void resetDisplayedImage(){
        if(!displayedImage.empty()){
            displayedImage.release();
            displayedImage = image->clone();
        }
    }

private:
    SidescanFile & file;

    cv::Mat * image = NULL;

    cv::Mat displayedImage;
    std::vector<SidescanPing * > * rawData = NULL;

    //channel info
    std::map<std::string,std::string>  * properties;

    unsigned int channelNumber;
    unsigned int channelType;
    std::string channelName;

    //object detection
    std::vector<InventoryObject *> objects;

    //Microfeatures
    std::vector<cv::KeyPoint> microFeatures;
};

#endif // SIDESCANIMAGE_H
