#include "telemetrymanager.h"
#include <QDebug>

//convert macro to string
#define str(x) #x
#define str1(x) str(x)
#define Version str1(PROJECT_VERSION)


TelemetryManager::TelemetryManager(QWidget *parent) : QWidget(parent){
    request->setUrl(QUrl("http://apps.cidco.ca/SBP-web-web/CheckLicense?code=OPENSIDESCAN-COMMUNITY"));
    /*
    os = QSysInfo::productType() + " " + QSysInfo::productVersion();
    kernel = QSysInfo::kernelType() + " " + QSysInfo::kernelVersion();
    gui = QGuiApplication::platformName();
    version = version.fromStdString(Version);
    qDebug()<<os;
    qDebug()<<kernel;
    qDebug()<<gui;
    qDebug()<<version;
    */
}
TelemetryManager::~TelemetryManager(){
    delete manager;
    delete request;
}

void TelemetryManager::send_telemetry(){

    reply = manager->get(*request);
    QEventLoop event;
    connect(reply, SIGNAL(finished()), &event, SLOT(quit()));
    event.exec();
    QString content = reply->readAll();

    QVariant statusCode = reply->attribute( QNetworkRequest::HttpStatusCodeAttribute );
    if (statusCode  != 403){
        qDebug()<< "The server understood the request, but will not fulfill it.";
    }
    else{
        qDebug()<<"keep searching for Atlantis";
    }


}
