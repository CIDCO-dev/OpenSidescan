#ifndef TELEMETRYMANAGER_H
#define TELEMETRYMANAGER_H

#include <QObject>
#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QGuiApplication>
#include <QSysInfo>

class TelemetryManager : public QWidget
{
    Q_OBJECT
public:
    explicit TelemetryManager(QWidget *parent = nullptr);
    ~TelemetryManager();
    void send_telemetry();


private:
    QNetworkAccessManager manager;
    QNetworkRequest request;
    //to be continued
    //QString os, gui, kernel, version;

};

#endif // TELEMETRYMANAGER_H
