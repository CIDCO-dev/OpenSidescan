/*
* Copyright 2019 © Centre Interdisciplinaire de développement en Cartographie des Océans (CIDCO), Tous droits réservés
*/

 /*
 * \author Christian Bouchard
 */

#ifndef WORKEROPENPROJECT_H
#define WORKEROPENPROJECT_H

#include <QVariant>

#include "project.h"

class WorkerOpenProject : public QObject
{
    Q_OBJECT

public:
    WorkerOpenProject( Project * project, std::string & sFilename );

//    virtual ~WorkerOpenProject(){}


public slots:
    void doWork();

signals:
    void done();

private:

    Project * project;
    std::string & sFilename;
};


#endif // WORKEROPENPROJECT_H

