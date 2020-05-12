#include "monitorthread.h"

#include <iostream>

MonitorThread::MonitorThread()
{

}

MonitorThread::~MonitorThread(){

}

void MonitorThread::process(){

    while(!exterminate){
        QThread::sleep(3);
        std::cout << "Ding" << std::endl;
    }

}

void MonitorThread::start(){
    this->moveToThread(&thread);
    //connect(this, SIGNAL (error(QString)), this, SLOT (errorString(QString)));
    connect(&thread, &QThread::started, this, &MonitorThread::process);
    //connect(this, &MonitorThread::finished()), &thread, SLOT (quit()));
    //connect(this, SIGNAL (finished()), this, SLOT (deleteLater()));
    //connect(&thread, SIGNAL (finished()), &thread, SLOT (deleteLater()));
    exterminate = false;
    thread.start();
}

void MonitorThread::stop(){
    exterminate = true;
    thread.quit();
    thread.wait();
}
