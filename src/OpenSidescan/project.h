#ifndef PROJECT_H
#define PROJECT_H

#include <vector>

#include <QString>

#include "sidescanfile.h"

#include "parameterscvCreateTrainingSamples.h"

class SidescanFile;
class SidescanImage;

class Project
{
public:
    Project();
    ~Project();

    std::vector<SidescanFile *> & getFiles() {return files;}

    void read(std::string & filename);

    void write(std::string & filename);

    void exportInventoryAsKml(std::string & filename);

    void exportInventoryAsCsv(std::string & filename);


    std::string & getFilename(){ return filename;}
    void          setFilename(std::string & fileName){ filename=fileName;}

    Eigen::Vector3d & getAntenna2TowPointLeverArm(){ return antenna2TowPointLeverArm;}
    void          setAntenna2TowPointLeverArm(Eigen::Vector3d leverArm){ antenna2TowPointLeverArm=leverArm;}

//    void saveObjectImages( const QString & folder );
    void saveObjectImages( const QString & absolutePath,
                           const QString & fileNameWithoutExtension );

    void createAndSaveTrainingObjectSamples( const QString & folder,
                                             const ParameterscvCreateTrainingSamples & parameterscvCreateTrainingSamples );

    void saveBackgroundImage( SidescanImage * image, const QString & folder, std::ofstream & outFile,
                                   int backgroundTop, int backgroundBottom );

    bool areThereFiles() const;
    bool areThereObjects() const;

    int computeNumberOfObjects() const;

private:

//    std::vector< std::pair <int,int> > computeObjectsVerticalOccupancy( SidescanImage * image );
    void computeObjectsVerticalOccupancy( SidescanImage * image,
                                          std::vector< std::pair <int,int> > & verticalPositions);


    std::vector<SidescanFile *>   files; 

    std::string filename;

    Eigen::Vector3d antenna2TowPointLeverArm;
};

#endif // PROJECT_H
