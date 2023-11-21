#include"proc.h"
#include <string>
#include "utils/timeutil.h"
using namespace std;

proc::proc() {
    name = QString("");
    createdTime=name;
    time = 0;
    needTime = 0;
    priority = 0;
    status = 0;
    memoryStart=0;
    /*memory = nullptr;*/
}
//proc::proc(const QString&name,const int&time,const int&priority):name(name),time(time),priority(priority){
//    createdTime=getNowTimeStr();
//    status=0;
//    needTime=time;
//}
proc::proc(const QString&name,const int&time,const int&priority,const int&asize):name(name),time(time),priority(priority),size(asize){
    createdTime=getNowTimeStr();
    status=0;
    needTime=time;
}
proc::proc(const QString& n, const QString& c,const int& t, const int& nt, const int& p, const int& s)
    : name(n), createdTime(c),time(t), needTime(nt), priority(p), status(s) {}
proc::~proc() {
    /*delete[]memory;*/
}
void proc::decNeedTime(){
    if(needTime>0)--needTime;
}
void proc::changePriority(const int&pri){
    priority=pri;
}
bool proc::isOver(){
    return needTime<=0;
}
int proc::getEnd(){
    return memoryStart+size;
}
bool proc::operator<(const proc&c)const{
    return (this->priority>c.priority)||((this->priority==c.priority)&&(this->needTime<c.needTime));
}
bool proc::operator>(const proc&c)const{
    return (this->priority)<(c.priority)||((this->priority==c.priority)&&(this->needTime>c.needTime));
}
ostream& operator<<(ostream& _cout, const proc& p)
{
    _cout << p.name.toStdString() << ',' << p.time << ',' << p.needTime << ',' << p.priority << ',' << p.status;
    return _cout;
}
void proc::tostr() {
    cout << "Name: " << name.toStdString() << ", ";
    cout << "Time: " << time << ", ";
    cout << "Need Time: " << needTime << ", ";
    cout << "Priority: " << priority << ", ";
    cout << "Status: " << (int)(status);
}
bool CompareProc::operator()(const proc* a, const proc* b)const{
    return *a < *b;
}
