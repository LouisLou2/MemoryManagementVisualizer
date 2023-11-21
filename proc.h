#ifndef APROCESS_H
#define APROCESS_H
#include <iostream>
#include <QString>

struct proc {
    //这里都没有使用指针，所以设置setter
    QString name;//进程名字
    QString createdTime;
    int time;//要求运行时间
    int needTime;//还需要运行的时间
    int priority;//优先级
    int status;//状态，0就绪，1运行，2阻塞.3结束
    int memoryStart=0;
    int size;
    proc();
    void decNeedTime();
    void changePriority(const int&pri);
    bool isOver();
    int getEnd();
    //此函数适合初始建立一个进程
    //proc(const QString&name,const int&time,const int&priority);
    proc(const QString&name,const int&time,const int&priority,const int&size);
    proc(const QString& n,const QString& c, const int& t, const int& nt, const int& p, const int& s);
    ~proc();
    bool operator<(const proc& p)const;
    bool operator>(const proc& p)const;
    friend std::ostream& operator<<(std::ostream& _cout, const proc& p);
    void tostr();
};

// 自定义比较函数，比较指针所指的对象
//在优先队列中按升序排列元素，较小的元素将被视为具有更高的优先级
struct CompareProc {
    bool operator()(const proc* a, const proc* b)const;
};
//根据内存起始地址排序
struct CompareByMemStart{
    bool operator()(const proc* const & a,const proc* const & b) const{
        return a->memoryStart<b->memoryStart;
    }
};
//CompareByMemStart_Func CompareByMemStart;
#endif // APROCESS_H
