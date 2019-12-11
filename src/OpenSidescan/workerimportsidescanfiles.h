/*
* Copyright 2019 © Centre Interdisciplinaire de développement en Cartographie des Océans (CIDCO), Tous droits réservés
*/

 /*
 * \author Christian Bouchard
 */

#ifndef WORKERIMPORTSIDESCANFILES_H
#define WORKERIMPORTSIDESCANFILES_H

#include <QDebug>


#include "projectwindow.h"


class WorkerImportSidescanFiles : public QObject
{
    Q_OBJECT

public:
    WorkerImportSidescanFiles( QStringList & fileNames,
                               ProjectWindow * projectWindow,
                               SidescanFile ** lastFile);

//    virtual ~WorkerImportSidescanFiles(){}

//    bool getSuccess() const;
//    bool getDidGetException() const;

    std::string getExceptionString() const;


public slots:
    void doWork();

signals:

//    void done();

    void progressInt(int);
    void progressFilename(QString);

private:

    QStringList & fileNames;
    ProjectWindow * projectWindow;
    SidescanFile ** lastFile;

//    bool success;

//    bool didGetException;

    std::string exceptionText;
};


#endif // WORKERIMPORTSIDESCANFILES_H
