#ifndef PROCESSMANAGER_H
#define PROCESSMANAGER_H
#include <unordered_map>
#include<deque>
#include<set>
#include <QListView>
#include "proc.h"
#include "cardwidget.h"
#include "ScrollableCardContainer.h"
#include <QTableWidget>
#include <QMessageBox>
#include "cpupara.h"
#include "memorymanager.h"
//此方法一举一动应该同时考虑内部数据结构和外部显示
class ProcessManager:public QObject
{
    Q_OBJECT
public:
    ProcessManager(CPUPara*aCpuPara,const std::string&preparedfile,ScrollableCardContainer*abackcon,
                                   ScrollableCardContainer*areadycon,
                                   ScrollableCardContainer*asuspendcon,
                                   ScrollableCardContainer*aswapcon,
                                    QTableWidget*aprocessingTable,
                                    QTableWidget* aprocessTable,
                                    QListView*adetailList,
                                    QObject*parent=nullptr);
    proc* getProcessInfo(const QString&name);
    void takeContainer(ScrollableCardContainer*backcon,
                       ScrollableCardContainer*readycon,
                       ScrollableCardContainer*suspendcon,
                       ScrollableCardContainer*swapcon,
                       QTableWidget*aprocessingTable,
                       QTableWidget* aprocessTable,
                       QListView*detailList);
    void takeMemoryManager(MemoryManager* amemoryManager);
    void registerAProcess(proc*process);
    void initBackQueue();

    int backSize();
    int readySize();
    int suspendSize();
    proc* checkPara(const QString&name,const QString&time,const QString&pri,const QString&size);
    ~ProcessManager();
public slots:
    void begin();
    void pause();
    void continueProcess();
    void addRows(const int& num);
    void readRowsFromFile();
    void addSuspendQueue();
    void addSuspendQueueFromProcessing();
    //传-1代表全部解挂
    void releaseFromSuspendQueue(const int&code=0);
    void revertToOrigin();
private slots:
    void invokeFunc();
    void addPreSuspendSet(const QString&name);
    void removeFromPreSuspendSet(const QString&name);
    void addPreReleaseSet(const QString&name);
    void removeFromPreReleaseSet(const QString&name);
private:
    //如果参数不符合规定，将返回nullptr
    proc* readArow(const int&rownum);
    void kickOnNext(const int&code,const int&interval);
    void backToReady(const int&n);
    //void readyToSuspend(const int&n);
    //void suspendToReady(const int&n);
    void drawACPUProcess(const int&coreid,const proc*process);
    void clearProcessingTable();
    void clearProcessTable(int rownum=-1);
    void reloadReady();
    void serveProcess();
    void handleRelease();
    void processingBackToReady();
    void addDetailInfo(proc*process);
    void clearDetailInfo();
public:
    std::string preparedfile;
    std::unordered_map<QString,proc*>processes;
    DerivedPriorityQueue<proc*,std::deque<proc*>,CompareProc> readyqueue;
    DerivedPriorityQueue<proc*,std::deque<proc*>,CompareProc> backqueue;
    std::set<proc*> swapqueue;
    std::deque<proc*> suspendqueue;
    std::deque<proc*>servingqueue;
    std::set<proc*> preSuspendSet;
    std::set<proc*> preReleaseSet;
    MemoryManager*memoryManager;
    //接管Window的可视化队列组件
    ScrollableCardContainer*backcon=nullptr;
    ScrollableCardContainer*readycon=nullptr;
    ScrollableCardContainer*suspendcon=nullptr;
    ScrollableCardContainer*swapcon=nullptr;
    QTableWidget* processingTable=nullptr;
    QTableWidget* processTable=nullptr;
    QListView*detailList;
    void setServeTime(const int&time);
private:
    CPUPara*cpupara;
    //系统进行进程读取，选择的时间(ms)
    int sysTime=1000;
    //一个进程每次在CPU上需要的时间
    int serveTime=3000;
    //计时器
    QTimer*timer;
    bool counting=false;//是否还在计时
    bool isProcessingTableClean=false;
    bool reloadedJustNow=false;
    bool reloadedFromBackJustNow=false;
    bool needHandleRelease=false;
    int releaseCode=0;
    int funcCode=-1;
    QMessageBox msgBox;
    void alertReleaseFailure(const std::vector<proc*>&alist);
};

#endif // PROCESSMANAGER_H
