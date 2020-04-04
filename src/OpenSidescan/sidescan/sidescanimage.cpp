#include "sidescanimage.h"
/*
SidescanImage::SidescanImage():image(NULL),rawData(NULL)
{

}
*/

SidescanImage::SidescanImage(
                            SidescanFile & file,
                            cv::Mat * img,
                            std::vector<SidescanPing * > * rawData,
                            unsigned int channelNumber,
                            std::string & channelName,
                            unsigned int channelType,
                            std::map<std::string,std::string>  * properties
                            )
                            : file(file),
                              image(img),
                              rawData(rawData),
                              displayedImage(img->clone()),
                              channelNumber(channelNumber),
                              channelName(channelName),
                              channelType(channelType),
                              properties(properties)
{

}

SidescanImage::~SidescanImage(){
    if(image) delete image;

    if(rawData){

        for(auto i=rawData->begin();i!=rawData->end();i++){
            delete (*i);
        }

        rawData->clear();
        delete rawData;
    }

    for(auto i=objects.begin();i!=objects.end();i++){
        delete (*i);
    }

    objects.clear();

    if(properties){
        delete properties;
    }
}


