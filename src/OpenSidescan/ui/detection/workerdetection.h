/*
* Copyright 2019 © Centre Interdisciplinaire de développement en Cartographie des Océans (CIDCO), Tous droits réservés
*/

 /*
 * \author Christian Bouchard,glm
 */

#ifndef WORKERDETECTION_H
#define WORKERDETECTION_H

#include "detector/detector.h"
#include "detectionwindow.h"
#include "project/project.h"

class WorkerDetection : public QObject
{
    Q_OBJECT

public:
    WorkerDetection( Project & project, DetectionWindow & detectionWindow, Detector & detector, unsigned long int &imageCount);

//    virtual ~WorkerDetection(){}

    std::string getExceptionString() const;

    void detectInFiles(SidescanFile & file);

public slots:
    void doWork();

signals:
    void progress(int);

private:
    Project & project;
    DetectionWindow & detectionWindow;
    Detector & detector;

    int fileIdx;
    unsigned long int imageCount;
};


#endif // WORKERDETECTION_H

