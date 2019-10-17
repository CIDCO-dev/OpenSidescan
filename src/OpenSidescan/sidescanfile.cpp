#include "sidescanfile.h"

SidescanFile::SidescanFile(std::string & filename): filename(filename),fileProperties(NULL)
{

}

SidescanFile::~SidescanFile(){
    for(auto i=images->begin();i!= images->end();i++){
        delete *i;
    }

    delete fileProperties;
}
