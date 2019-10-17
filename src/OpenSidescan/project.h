#ifndef PROJECT_H
#define PROJECT_H

#include <vector>
#include "sidescanfile.h"

class Project
{
public:
    Project();
    ~Project();

    std::vector<SidescanFile *> & getFiles() {return files;}

private:
    std::vector<SidescanFile *>   files;
};

#endif // PROJECT_H
