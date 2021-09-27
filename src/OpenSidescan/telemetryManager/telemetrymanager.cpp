#include "telemetrymanager.h"
#include <QDebug>


TelemetryManager::TelemetryManager(QWidget *parent) : QWidget(parent){
    //QNetworkAccessManager *manager;
    //QNetworkRequest *request ;
    request->setUrl(QUrl("https://www.google.ca/")); // "http://apps.cidco.ca/SBP-web-web/CheckLicense?code=OPENSIDESCAN-COMMUNITY"
}

void TelemetryManager::send_telemetry(){

    reply = manager->get(*request);
    QEventLoop event;
    connect(reply, SIGNAL(finished()), &event, SLOT(quit()));
    event.exec();
    QString content = reply->readAll();

    qDebug()<<content;
}
