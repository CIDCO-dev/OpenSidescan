#ifndef TELEMETRYMANAGER_H
#define TELEMETRYMANAGER_H

#include <QObject>
#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>

class TelemetryManager : public QWidget
{
    Q_OBJECT
public:
    explicit TelemetryManager(QWidget *parent = nullptr);
    ~TelemetryManager();
    void send_telemetry();


private:
    QNetworkAccessManager * manager = new QNetworkAccessManager;
    QNetworkRequest * request = new QNetworkRequest();
    QNetworkReply * reply;

};

#endif // TELEMETRYMANAGER_H
