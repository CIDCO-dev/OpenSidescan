/*
* Copyright 2019 © Centre Interdisciplinaire de développement en Cartographie des Océans (CIDCO), Tous droits réservés
*/

 /*
 * \author Christian Bouchard
 */

#ifndef WORKERTRAININGSAMPLES_H
#define WORKERTRAININGSAMPLES_H

#include <utility>      // std::pair, std::make_pair

#include <QObject>

#include <QString>

#include "project/project.h"

#include "parameterscvCreateTrainingSamples.h"

#include "refactorme/boolwithmutex.h"

class WorkerTrainingSamples : public QObject
{
    Q_OBJECT

public:
    WorkerTrainingSamples( Project * project,
                            const int numberOfObjects,
                            const ParameterscvCreateTrainingSamples & parameters,

                            const QString & folderOriginalObjectImages,
                            const QString & folderOutputPositiveSamples,
                            const QString & folderBackground,

                            std::ofstream & outFile,
                            BoolWithMutex * continueWhatYourDoing );

//    virtual ~WorkerTrainingSamples(){}

    void computeMaxDimensions(SidescanFile & file);
    void saveBackgrounds(SidescanFile & file);
    void saveSamples(SidescanFile & file);

public slots:
    void doWork();

signals:
    void progress(int);
    void done();

private:

    void saveBackgroundImage( SidescanImage * image, const QString & folder,
                              std::ofstream & outFile,
                              int backgroundTop, int backgroundBottom );

    void computeObjectsVerticalOccupancy( SidescanImage * image,
                                          std::vector< std::pair <int,int> > & verticalPositions);



    Project * project;

    int maxObjectWidth = 0;
    int maxObjectHeight = 0;

    int minBackgroundWidth = 0;
    int minBackgroundHeight = 0;

    int numberOfObjects =0;

    int countObjects =0;

    const ParameterscvCreateTrainingSamples & parameters;

    const QString & folderOriginalObjectImages;
    const QString & folderOutputPositiveSamples;
    const QString & folderBackground;

    std::ofstream & outFile;

    BoolWithMutex * continueWhatYourDoing;
};

#endif // WORKERTRAININGSAMPLES_H
