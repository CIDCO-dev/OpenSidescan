/*
* Copyright 2019 © Centre Interdisciplinaire de développement en Cartographie des Océans (CIDCO), Tous droits réservés
*/

 /*
 * \author Christian Bouchard,glm
 */

#include <QDebug>


#include "workerdetection.h"


WorkerDetection::WorkerDetection(Project & project, DetectionWindow & detectionWindow, Detector & detector )
    : project(project),detectionWindow( detectionWindow ),detector(detector)
{
}

void WorkerDetection::detectInFiles(SidescanFile & file){
    for(auto j=file.getImages().begin();j != file.getImages().end();j++){
        detector.detect(**j,(*j)->getObjects());
        progress(fileIdx++);
    }
}


void WorkerDetection::doWork() {
    fileIdx = 0 ;

    project.walkFiles<WorkerDetection>(this,&WorkerDetection::detectInFiles);

    progress(project.getFileCount()); // To close the progress dialog
}
