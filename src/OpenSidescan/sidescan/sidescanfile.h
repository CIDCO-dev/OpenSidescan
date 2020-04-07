#ifndef SIDESCANFILE_H
#define SIDESCANFILE_H

#include <vector>
#include "sidescanimage.h"
#include <Eigen/Dense>

class SidescanImage;

class SidescanFile
{
public:
    SidescanFile(std::string & filename, Eigen::Vector3d antenna2TowPointLeverArm);
    ~SidescanFile();

    std::vector<SidescanImage*>  & getImages(){ return *images;}
    void setImages(std::vector<SidescanImage*> * img) { images=img;}

    std::string & getFilename(){ return filename;}

    std::map<std::string,std::string> & getFileProperties(){ return *fileProperties;}
    void setFileProperties(std::map<std::string,std::string> * fp){ fileProperties = fp;}

    double getAverageXDistancePerPixel() { return averageXDistancePerPixel;}
    void   setAverageXDistancePerPixel(double avgDistance) { averageXDistancePerPixel = avgDistance;}

    Eigen::Vector3d & getAntenna2TowPointLeverArm() {return antenna2TowPointLeverArm;}

private:
    std::string                 filename;

    std::vector<SidescanImage*> * images = NULL;

    std::map<std::string,std::string> * fileProperties;

    double averageXDistancePerPixel = 0;

    Eigen::Vector3d antenna2TowPointLeverArm;
};

#endif // SIDESCANFILE_H
