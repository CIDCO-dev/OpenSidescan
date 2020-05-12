#ifndef MONITORTHREAD_H
#define MONITORTHREAD_H

#include <QThread>

class MonitorThread : public QObject
{
    Q_OBJECT
public:
    MonitorThread();
    ~MonitorThread();

    void start();
    void stop();

    void process();

private:
    QThread thread;
    bool exterminate=false; //Exterminate. Annihilate. DESTROY.
};

#endif // MONITORTHREAD_H
