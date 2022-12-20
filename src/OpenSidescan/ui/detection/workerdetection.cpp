/*
* Copyright 2019 © Centre Interdisciplinaire de développement en Cartographie des Océans (CIDCO), Tous droits réservés
*/

 /*
 * \author Christian Bouchard,glm
 */

#include <QDebug>


#include "workerdetection.h"


WorkerDetection::WorkerDetection(Project & project, DetectionWindow & detectionWindow, Detector & detector, unsigned long int &imageCount)
    : project(project),detectionWindow( detectionWindow ),detector(detector), imageCount(imageCount)
{
}

void WorkerDetection::detectInFiles(SidescanFile & file){
    for(auto j=file.getImages().begin();j != file.getImages().end();j++){
        progress(fileIdx++);
        detector.detect(**j,(*j)->getObjects());
    }
}


void WorkerDetection::doWork() {
    fileIdx = 0 ;

    project.walkFiles<WorkerDetection>(this,&WorkerDetection::detectInFiles);

    progress(imageCount); // To close the progress dialog
}
