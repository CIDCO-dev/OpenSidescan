/*
* Copyright 2019 © Centre Interdisciplinaire de développement en Cartographie des Océans (CIDCO), Tous droits réservés
*/

 /*
 * \author Christian Bouchard
 */

#include <QDebug>

#include "workeropenproject.h"

WorkerOpenProject::WorkerOpenProject( Project * project, std::string & sFilename )
    : project( project ), sFilename( sFilename )
{
}

void WorkerOpenProject::doWork() {

    project->read(sFilename);

    emit done();

//    qDebug() << tr( "End of DoWork" );
}
