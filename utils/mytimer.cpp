#include "mytimer.h"
#include <QDateTime>
#include <iostream>
myTimer::myTimer(QObject*parent):QObject(parent)
{
    timer=new QTimer;
    remaining=0;
    interval=0;
    running=false;
    connect(timer,&QTimer::timeout,this,&myTimer::getInnerTimeOut);
}
void myTimer::start(const int&interval){
    if(interval!=-1)this->interval=interval;
    remaining=0;
    running=true;
    timer->start(interval);
    exStartTime = QDateTime::currentDateTime();
}
void myTimer::setInterval(const int&interval){
    this->interval=interval;
}
void myTimer::continueCounting(){
    if(remaining<0)return;
    running=true;
    timer->start(remaining);
}
void myTimer::setSingleShot(bool single){
    timer->setSingleShot(single);
}
void myTimer::stop(){
    if(!running)return;
    timer->stop();
    running=false;
    int consuming=exStartTime.msecsTo(QDateTime::currentDateTime());
    remaining=interval-consuming;
    std::cout<<"remaining:"<<remaining<<std::endl;
}
void myTimer::getInnerTimeOut(){
    if(single)running=false;
    emit timeout();
}
myTimer::~myTimer(){
    delete timer;
}
