#define OPENCV_TRAITS_ENABLE_DEPRECATED

#include "ui/mainwindow/mainwindow.h"
#include "telemetryManager/telemetrymanager.h"
#include <QApplication>
#include <QSplashScreen>

#include <QFile>
#include <QTextStream>

//TODO: windows-proof this (for sleep())

#ifdef _WIN32
#include <windows.h>
#define sleep Sleep
#else
#include <unistd.h>
#endif

const char * captions[] = {
    "Obtaining Poseidon's blessing...",
    "Initializing Queequeg engine...",
    "Summoning great white whale...",
    "Reticulating splines...",
    "Cleaning up the oceans...",
    "Launching asynchronous dolphin call...",
    "Parity-checking barnacles...",
    "Removing fishbones...",
    "Bootstrapping pacemaker with magnetometer...",
    "Hiring Captain Ahab...",
    "Ph'nglui mglw'nafh Cthulhu R'lyeh wgah'nagl fhtagn",
    "Applying sunscreen...",
    "Increasing sunspot activity...",
    "Waking up Cthulhu...",
    "Recalibrating lobster traps...",
    "Freeing up memory from lost fishing nets..."
};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //Show splash
    srand(time(NULL));
    QPixmap splashImage (":/Images/resources/splash.jpg");
    QSplashScreen splash(splashImage);


    splash.show();
    sleep(1);

    TelemetryManager user;
    user.send_telemetry();
    user.~TelemetryManager();


    for(int i=0;i<3;i++){
        splash.showMessage(captions[rand()%16],Qt::AlignHCenter|Qt::AlignBottom,QColor("white"));
        sleep(1);
    }

    a.processEvents();

    sleep(1);

    splash.hide();


    //Load stylesheet
    QFile f(":qdarkstyle/style.qss");
    if (!f.exists())
    {
        printf("Unable to set stylesheet, file not found\n");
    }
    else
    {
        f.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&f);
        qApp->setStyleSheet(ts.readAll());
    }

    //Show main window
    MainWindow w;
    w.showMaximized();

    return a.exec();
}
