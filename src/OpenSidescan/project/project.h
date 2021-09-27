#ifndef PROJECT_H
#define PROJECT_H

#include <vector>
#include <mutex>

#include <QString>

#include "sidescan/sidescanfile.h"

#include "ui/training/parameterscvCreateTrainingSamples.h"

class SidescanFile;
class SidescanImage;

class Project
{
public:
    Project();
    ~Project();

    void read(std::string & filename);

    void write(std::string & filename);

    void exportInventoryAsKml(std::string & filename);

    void exportInventoryAsCsv(std::string & filename);
    
    void exportInventoryAsHits(std::string & filename);

    void exportInventory4PyTorch(std::string & filename);

    std::string & getFilename(){ return filename;}
    void          setFilename(std::string & fileName){ filename=fileName;}

    Eigen::Vector3d & getAntenna2TowPointLeverArm(){ return antenna2TowPointLeverArm;}
    void              setAntenna2TowPointLeverArm(double x,double y, double z){
        mutex.lock();
        antenna2TowPointLeverArm << x,y,z;
        mutex.unlock();
    }

    void saveObjectImages( const QString & absolutePath, const QString & fileNameWithoutExtension );

    void createAndSaveTrainingObjectSamples( const QString & folder, const ParameterscvCreateTrainingSamples & parameterscvCreateTrainingSamples );

    void saveBackgroundImage( SidescanImage * image, const QString & folder, std::ofstream & outFile, int backgroundTop, int backgroundBottom );

    void addFile(SidescanFile * newFile);
    void removeFile(SidescanFile * file);

    unsigned long getFileCount();
    unsigned long getObjectCount();

    /**
     * A generic visitor method that wraps up the proper use of the project's mutex to avoid race conditions between GUI operations and Monitor-mode
     */
    template <typename T>
    void walkFiles(T * obj,void (T::*fn)(SidescanFile &)){
        mutex.lock();

        for(auto i = files.begin();i != files.end();i++){
            (obj->*fn)(*(*i));
        }

        mutex.unlock();
    }

    bool containsFile(std::string & filename);



private:

    void computeObjectsVerticalOccupancy( SidescanImage * image, std::vector< std::pair <int,int> > & verticalPositions);


    std::vector<SidescanFile *>   files; 

    std::string filename;

    Eigen::Vector3d antenna2TowPointLeverArm;

    std::mutex mutex; //used to control access between GUI-mode and monitor-mode
};

#endif // PROJECT_H
