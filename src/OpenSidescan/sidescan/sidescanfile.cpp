#include "sidescan/sidescanfile.h"

SidescanFile::SidescanFile(std::string & filename, Eigen::Vector3d antenna2TowPointLeverArm): filename(filename), fileProperties(NULL), antenna2TowPointLeverArm(antenna2TowPointLeverArm)
{

}

SidescanFile::~SidescanFile(){
    for(auto i=images->begin();i!= images->end();i++){
        delete *i;
    }

    delete fileProperties;
}
