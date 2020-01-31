#ifndef PROJECT_H
#define PROJECT_H

#include <vector>

#include <QString>

#include "sidescanfile.h"

class Project
{
public:
    Project();
    ~Project();

    std::vector<SidescanFile *> & getFiles() {return files;}

    void read(std::string & filename);

    void write(std::string & filename);

    void exportInventoryAsKml(std::string & filename);

    std::string & getFilename(){ return filename;}
    void          setFilename(std::string & fileName){ filename=fileName;}

    void saveObjectImages( const QString & folder );

private:
    std::vector<SidescanFile *>   files; 

    std::string filename;
};

#endif // PROJECT_H
