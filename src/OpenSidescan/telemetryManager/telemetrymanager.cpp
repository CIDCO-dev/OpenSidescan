#include "telemetrymanager.h"
#include <QDebug>


TelemetryManager::TelemetryManager(QWidget *parent) : QWidget(parent){
    request->setUrl(QUrl("https://www.google.ca/")); // "http://apps.cidco.ca/SBP-web-web/CheckLicense?code=OPENSIDESCAN-COMMUNITY"
}

TelemetryManager::~TelemetryManager(){
    delete manager;
    delete request;
}

void TelemetryManager::send_telemetry(){
    /*
    reply = manager->get(*request);
    QEventLoop event;
    connect(reply, SIGNAL(finished()), &event, SLOT(quit()));
    event.exec();
    QString content = reply->readAll();

    qDebug()<<content;
    */
    manager->get(*request);
    QObject::connect(manager, &QNetworkAccessManager::finished,this, [=](QNetworkReply *reply) {
                if (reply->error()) {
                    qDebug() << reply->errorString();
                    return;
                }

                QString answer = reply->readAll();

                qDebug() << answer;
            }
        );

}
