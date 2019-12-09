#ifndef SIDESCANFILE_H
#define SIDESCANFILE_H

#include <vector>
#include "sidescanimage.h"

class SidescanImage;

class SidescanFile
{
public:
    SidescanFile(std::string & filename);
    ~SidescanFile();

    std::vector<SidescanImage*>  & getImages(){ return *images;}
    void setImages(std::vector<SidescanImage*> * img) { images=img;}

    std::string & getFilename(){ return filename;}

    std::map<std::string,std::string> & getFileProperties(){ return *fileProperties;}
    void setFileProperties(std::map<std::string,std::string> * fp){ fileProperties = fp;}

    double getAverageXDistancePerPixel() { return averageXDistancePerPixel;}
    void   setAverageXDistancePerPixel(double avgDistance) { averageXDistancePerPixel = avgDistance;}

private:
    std::string                 filename;

    std::vector<SidescanImage*> * images = NULL;

    std::map<std::string,std::string> * fileProperties;

    double averageXDistancePerPixel = 0;
};

#endif // SIDESCANFILE_H
