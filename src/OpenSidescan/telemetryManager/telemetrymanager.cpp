#include "telemetrymanager.h"
#include <QDebug>


TelemetryManager::TelemetryManager(QWidget *parent) : QWidget(parent){
    os = QSysInfo::productType() + " " + QSysInfo::productVersion();
    kernel = QSysInfo::kernelType() + " " + QSysInfo::kernelVersion();
    gui = QGuiApplication::platformName();
    version = "todo";
    request->setUrl(QUrl("http://apps.cidco.ca/SBP-web-web/CheckLicense?code=OPENSIDESCAN-COMMUNITY"));

    qDebug()<<os;
    qDebug()<<kernel;
    qDebug()<<gui;
    qDebug()<<version;
}
TelemetryManager::~TelemetryManager(){
    //delete manager;
    //delete request;
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
