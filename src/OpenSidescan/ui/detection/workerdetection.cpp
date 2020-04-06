/*
* Copyright 2019 © Centre Interdisciplinaire de développement en Cartographie des Océans (CIDCO), Tous droits réservés
*/

 /*
 * \author Christian Bouchard
 */

#include <QDebug>


#include "workerdetection.h"


WorkerDetection::WorkerDetection( DetectionWindow & detectionWindow, Detector & detector )
    : detectionWindow( detectionWindow ),detector(detector)
{
}


void WorkerDetection::doWork() {

    std::vector<SidescanFile *>  & files = detectionWindow.getFiles();

    int fileIdx = 0;
    for(auto i=files.begin();i!=files.end();i++){
        //for every file, every image

        for(auto j=(*i)->getImages().begin();j != (*i)->getImages().end();j++){
            detector.detect(**j,(*j)->getObjects());
/*
                        detectionWindow->getFastThresholdValue(),
                        detectionWindow->getFastTypeValue(),
                        detectionWindow->getFastNonMaxSuppressionValue(),
                        detectionWindow->getDbscanEpsilonValue(),
                        detectionWindow->getDbscanMinPointsValue(),
                        detectionWindow->getMserDeltaValue(),
                        detectionWindow->getMserMinimumAreaValue(),
                        detectionWindow->getMserMaximumAreaValue(),
                        detectionWindow->getMergeOverlappingBoundingBoxesValue()
*/
        }

        progress(fileIdx);

        fileIdx++;

    }

    progress( files.size()); // To close the progress dialog
}
