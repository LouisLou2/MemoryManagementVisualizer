#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H
#include<set>
#include <list>
#include <QTableWidget>
#include "proc.h"
#include "memorywidget.h"
#include "memoryblock.h"
#include "memoryusageprogressbar.h"
#include "memoryguimanager.h"
class MemoryManager: public QObject
{
    Q_OBJECT
private:
    std::set<proc*,CompareByMemStart>*memSet;
    std::list<MemoryBlock*>*freeMemList;
    MemoryGuiManager*memguiManager;
    int usingSize;
    bool (MemoryManager::*allocateFunc)(proc*);
    bool FirstFit(proc*process);
    bool BestFit(proc*process);
public slots:
    void changeAllocateAlgo(int index);
public:
    MemoryManager(QObject*parent=nullptr);
    MemoryManager(QLabel*allmemLabel,QLabel*memUsingLabel,QLabel*memLeftLabel,MemoryUsageProgressBar*membar,MemoryWidget*memwidget,QTableWidget*memDetailTable,QObject*parent=nullptr);
    void initManager();
    bool allocateForAProcess(proc*process);
    void recycleForAProcess(proc*process);
    void revertToOrigin();
    void changeAllMemSize();
signals:
    void MemoryWidgetNeedUpdate(const int&usingSize);
};

#endif // MEMORYMANAGER_H
