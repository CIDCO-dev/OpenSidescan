/*
* Copyright 2019 © Centre Interdisciplinaire de développement en Cartographie des Océans (CIDCO), Tous droits réservés
*/

 /*
 * \author Christian Bouchard
 */

#ifndef WORKERIMPORTSIDESCANFILES_H
#define WORKERIMPORTSIDESCANFILES_H

#include "projectwindow.h"

class WorkerImportSidescanFiles : public QObject
{
    Q_OBJECT

public:
    WorkerImportSidescanFiles( QStringList & fileNames,
                               ProjectWindow * projectWindow,
                               SidescanFile ** lastFile);

//    virtual ~WorkerImportSidescanFiles(){}

    std::string getExceptionString() const;

public slots:
    void doWork();

signals:
    void progressInt(int);
    void progressFilename(QString);

    void fileToBeAddedToProjectWindow( SidescanFile * file );

private:

    QStringList & fileNames;
    ProjectWindow * projectWindow;
    SidescanFile ** lastFile;

    std::string exceptionText;
};


#endif // WORKERIMPORTSIDESCANFILES_H
