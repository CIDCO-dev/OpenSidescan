/*
* Copyright 2019 © Centre Interdisciplinaire de développement en Cartographie des Océans (CIDCO), Tous droits réservés
*/

 /*
 * \author Christian Bouchard
 */

#include <QDebug>


#include "workerdetection.h"


WorkerDetection::WorkerDetection( DetectionWindow * detectionWindow )
    : detectionWindow( detectionWindow )
{
}


void WorkerDetection::doWork() {

    std::vector<SidescanFile *>  & files = detectionWindow->getFiles();

    int fileIdx = 0;
    for(auto i=files.begin();i!=files.end();i++){
        //for every file, every image

        for(auto j=(*i)->getImages().begin();j != (*i)->getImages().end();j++){
            OpencvHelper::detectObjects(
                        (*j)->getObjects(),
                        **i,
                        **j,

                        detectionWindow->getFastThresholdValue(),
                        detectionWindow->getFastTypeValue(),
                        detectionWindow->getFastNonMaxSuppressionValue(),
                        detectionWindow->getDbscanEpsilonValue(),
                        detectionWindow->getDbscanMinPointsValue(),
                        detectionWindow->getMserDeltaValue(),
                        detectionWindow->getMserMinimumAreaValue(),
                        detectionWindow->getMserMaximumAreaValue(),
                        detectionWindow->getShowFeatureMarkersValue(),
                        detectionWindow->getMergeOverlappingBoundingBoxesValue()
                    );
        }

        progress(fileIdx);

        fileIdx++;

    }

    progress( files.size()); // To close the progress dialog
}
