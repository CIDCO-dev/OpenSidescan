#include "mainwindow.h"
#include <QApplication>
#include <QSplashScreen>

//TODO: windows-proof this (for sleep())
#include <unistd.h>

const char * captions[] = {
    "Obtaining Poseidon's blessing...",
    "Initializing Queequeg engine...",
    "Summoning great white whale...",
    "Reticulating splines...",
    "Cleaning up the oceans...",
    "Launching asynchronous dolphin call...",
    "Parity-checking barnacles...",
    "Removing fishbones from backbone...",
    "Bootstrapping pacemaker with magnetometer...",
    "Stroking the Big Red Button...",
    "Warming up planet Earth...",
    "Increasing sunspot activity...",
    "Recalibrating lobster traps...",
    "Freeing up memory from lost fishing nets..."
};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QPixmap splashImage (":/Images/resources/splash.jpg");
    QSplashScreen splash(splashImage);

    splash.show();
    sleep(1);

    for(int i=0;i<3;i++){
        splash.showMessage(captions[rand()%14],Qt::AlignHCenter|Qt::AlignBottom,QColor("white"));
        sleep(1);
    }

    a.processEvents();

    sleep(1);

    MainWindow w;
    w.showFullScreen();

    return a.exec();
}
