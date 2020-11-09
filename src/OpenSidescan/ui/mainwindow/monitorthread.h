#ifndef MONITORTHREAD_H
#define MONITORTHREAD_H

#include <QDir>
#include <QThread>

#include "detector/detector.h"
#include "project/project.h"

class MainWindow;

class MonitorThread : public QObject
{
    Q_OBJECT
public:
    MonitorThread(QDir directory, Project * project, Detector * detector);
    ~MonitorThread();



    void start();
    void stop();

    void process();

signals:
    void fileToBeAddedToProjectWindow( SidescanFile * file );
    void updateProgress( QString progress );

private:
    QThread thread;
    bool exterminate=false; //Exterminate. Annihilate. DESTROY.



    QDir directory;
    Project * project;
    Detector * detector;

    bool fileCanBeChecked(std::string filename);
};

#endif // MONITORTHREAD_H
