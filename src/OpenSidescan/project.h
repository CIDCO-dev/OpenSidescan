#ifndef PROJECT_H
#define PROJECT_H

#include <vector>

#include <QString>

#include "sidescanfile.h"

#include "parameterscvCreateTrainingSamples.h"


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

//    void saveObjectImages( const QString & folder );
    void saveObjectImages( const QString & absolutePath,
                           const QString & fileNameWithoutExtension );

    void createAndSaveTrainingObjectSamples( const QString & folder,
                                             const ParameterscvCreateTrainingSamples & parameterscvCreateTrainingSamples );

    void saveBackgroundImage( SidescanImage * image, const QString & folder, std::ofstream & outFile,
                                   int backgroundTop, int backgroundBottom );

    bool areThereFiles() const;
    bool areThereObjects() const;

private:

//    std::vector< std::pair <int,int> > computeObjectsVerticalOccupancy( SidescanImage * image );
    void computeObjectsVerticalOccupancy( SidescanImage * image,
                                          std::vector< std::pair <int,int> > & verticalPositions);


    std::vector<SidescanFile *>   files; 

    std::string filename;
};

#endif // PROJECT_H
