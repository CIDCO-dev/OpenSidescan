/*
* Copyright 2019 © Centre Interdisciplinaire de développement en Cartographie des Océans (CIDCO), Tous droits réservés
*/

 /*
 * \author Christian Bouchard
 */

#ifndef WORKERDETECTION_H
#define WORKERDETECTION_H

#include "detector/detector.h"
#include "detectionwindow.h"

class WorkerDetection : public QObject
{
    Q_OBJECT

public:
    WorkerDetection( DetectionWindow & detectionWindow,Detector & detector );

//    virtual ~WorkerDetection(){}

    std::string getExceptionString() const;

public slots:
    void doWork();

signals:
    void progress(int);

private:

    DetectionWindow & detectionWindow;
    Detector & detector;
};


#endif // WORKERDETECTION_H

