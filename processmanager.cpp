#include "processmanager.h"
#include "utils/parseutil.h"
#include "myitem.h"
#include <QTimer>
#include <chrono>
#include <QFile>
#include <QFileDialog>
#include <QDebug>
#include <QStandardItemModel>

ProcessManager::ProcessManager(CPUPara*aCpuPara,const std::string&preparedfile,ScrollableCardContainer*abackcon,
                               ScrollableCardContainer*areadycon,
                               ScrollableCardContainer*asuspendcon, ScrollableCardContainer*aswapcon,QTableWidget*aprocessingTable,QTableWidget* aprocessTable,QListView*adetailList,QObject*parent):QObject(parent),preparedfile(preparedfile),cpupara(aCpuPara){
    takeContainer(abackcon,areadycon,asuspendcon,aswapcon,aprocessingTable,aprocessTable,adetailList);
    initBackQueue();
    timer=new QTimer;
    connect(timer,&QTimer::timeout,this,&ProcessManager::invokeFunc);
}
ProcessManager::~ProcessManager(){
    for(auto &it:processes){
        delete it.second;
    }
    processes.clear();
}
void ProcessManager::registerAProcess(proc*process){
    processes[process->name]=process;
    addDetailInfo(process);
}
proc* ProcessManager::getProcessInfo(const QString&name){
    return processes[name];
}
void ProcessManager::initBackQueue(){
    std::vector<proc*>vec;
    readfile(preparedfile,vec,this,this->checkPara);
    int num=0;
    for(proc*& item:vec){
        backqueue.push(std::move(item));
        registerAProcess(item);
        ++num;
    }
    backcon->updateByQueue(backqueue);
}
void ProcessManager::takeContainer(ScrollableCardContainer*abackcon,
                   ScrollableCardContainer*areadycon,
                   ScrollableCardContainer*asuspendcon,
                   ScrollableCardContainer*aswapcon,
                   QTableWidget*aprocessingTable,
                   QTableWidget* aprocessTable,
                   QListView*detailList){
    this->backcon=abackcon;
    this->readycon=areadycon;
    this->suspendcon=asuspendcon;
    this->swapcon=aswapcon;
    connect(areadycon,&ScrollableCardContainer::CardClicked,this,&ProcessManager::addPreSuspendSet);
    connect(areadycon,&ScrollableCardContainer::BreakCardClicked,this,&ProcessManager::removeFromPreSuspendSet);
    connect(suspendcon,&ScrollableCardContainer::CardClicked,this,&ProcessManager::addPreReleaseSet);
    connect(suspendcon,&ScrollableCardContainer::BreakCardClicked,this,&ProcessManager::removeFromPreReleaseSet);
    this->processingTable=aprocessingTable;
    this->processTable=aprocessTable;
    this->detailList=detailList;
}
void ProcessManager::takeMemoryManager(MemoryManager* amemoryManager){
    memoryManager=amemoryManager;
}
int ProcessManager::backSize(){return backqueue.size();}
int ProcessManager::readySize(){return readyqueue.size();}
int ProcessManager::suspendSize(){return suspendqueue.size();}
void ProcessManager::setServeTime(const int&time){
    serveTime=time;
}
//参数-1记为全部转移
void ProcessManager::backToReady(const int&n){
    int num=(n==-1)?INT_MAX:n;//即使这里提出想要尽可能多读入的请求，也不能多于并行道数
    int i=0;
    int j=0;
    proc*aproc;
    int backlen=backqueue.size();
    while(i<num&&j<backlen&&readyqueue.size()<cpupara->getcpuParallelThreads()&&(!backqueue.empty())&&(!backcon->empty())){
        aproc=backqueue.top();
        if(!memoryManager->allocateForAProcess(aproc)){
            ++j;
            continue;
        }
        readyqueue.push(aproc);
        backqueue.pop();
        backcon->pop();
        ++i;
        ++j;
    }
}
//参数-1记为全部转移
//void ProcessManager::readyToSuspend(const int&n){
//    int num=(n==-1)?INT_MAX:n;
//    int i=0;
//    proc*aproc;
//    while(i<num&&!readyqueue.empty()&&!readycon->empty()){
//        aproc=readyqueue.top();
//        suspendqueue.push_back(aproc);
//        readyqueue.pop();
//        readycon->pop();
//        memoryManager->recycleForAProcess(aproc);
//        ++i;
//    }
//    suspendcon->updateByQueue(suspendqueue);
//}
//参数-1记为全部转移
//void ProcessManager::suspendToReady(const int&n){
//    int num=(n==-1)?INT_MAX:n;
//    int i=0;
//    proc*aproc;
//    while(i<num&&!suspendqueue.empty()&&!suspendcon->empty()){
//        aproc=suspendqueue[0];
//        if(!memoryManager->allocateForAProcess(aproc))continue;//这里由于内存不足无法解挂的进程还留在挂起队列
//        readyqueue.push(suspendqueue[0]);
//        suspendqueue.pop_front();
//        suspendcon->pop();
//        ++i;
//    }
//    readycon->updateByQueue(readyqueue);
//}
void ProcessManager::begin(){
    kickOnNext(2,sysTime);
}
void ProcessManager::pause(){
    timer->stop();
}
void ProcessManager::continueProcess(){
    timer->setSingleShot(true);
    timer->start();
}
void ProcessManager::reloadReady(){
    backToReady(-1);
}
//考虑：servingqueue中的进程再次进入就绪队列需要再次allocate吗？因为只要他在就绪队列或者servingqueue,就不会剥夺它的内存
void ProcessManager::processingBackToReady(){
    for(const auto& it:servingqueue){
        readyqueue.push(it);
    }
    servingqueue.clear();
//    for (auto it = servingqueue.begin(); it != servingqueue.end(); ++it) {
//        readyqueue.push(std::move(*it));
//    }
}
void ProcessManager::serveProcess(){
    clearProcessingTable();
    //如果到了下一次，该有进程needTime还有，则再次希望先装填，停顿sysTime后，再进行下次cpu选择。
    if(!reloadedJustNow){

        for(auto it=servingqueue.begin();it!=servingqueue.end();){
            if((*it)->isOver()){
                memoryManager->recycleForAProcess(*it);
                it=servingqueue.erase(it);
            }else{
                std::advance(it,1);
            }
        }
        proc*aproc=nullptr;
        //当有进程结束了，开始检查腾出的空间够不够swap区的进程进入
        for(auto it=swapqueue.begin();it!=swapqueue.end();){
            aproc=*(it);
            if(memoryManager->allocateForAProcess(aproc)){
                readyqueue.push(aproc);
                swapqueue.erase(it++);
            }else{
                std::advance(it,1);
            }
        }
        if(!servingqueue.empty()){
            processingBackToReady();
            reloadedJustNow=true;
        }
        if(needHandleRelease){
           handleRelease();
           needHandleRelease=false;
        }
        if(readyqueue.size()<cpupara->getcpuParallelThreads()){
            reloadReady();
            reloadedFromBackJustNow=true;
            reloadedJustNow=true;
        }
        if(reloadedFromBackJustNow){
            backcon->updateByQueue(backqueue);
            reloadedFromBackJustNow=false;
        }
        swapcon->updateByQueue(swapqueue);
        readycon->updateByQueue(readyqueue);
        readycon->updateClickedState(preSuspendSet);
        suspendcon->updateByQueue(suspendqueue);
        reloadedJustNow=true;
        kickOnNext(2,serveTime);
        return;
    }
    int i=1;
    proc*process=nullptr;
    while(!readyqueue.empty()&& i<=cpupara->getCoreCount()){
        if(readyqueue.size()!=readycon->size()){
            std::cout<<"[严重]readyqueue.size()!=readycon->size()   已主动结束"<<std::endl;/*debug------------------------如果实在没有问题就删掉*/
            exit(1);
        }
        process=readyqueue.top();
        process->decNeedTime();
        process->changePriority(process->priority+1);
        //若进程被选入将要执行，所以它的被选中状态要抹除
        if(preSuspendSet.count(process)>0){
            preSuspendSet.erase(process);
            //readycon->eraseIfClicked(process->name);
        }
        drawACPUProcess(i,process);
        servingqueue.push_back(process);

        readyqueue.pop();
        readycon->pop();
        ++i;
    }
    reloadedJustNow=false;
    kickOnNext(2,serveTime);
}
void ProcessManager::drawACPUProcess(const int&coreid,const proc*process){
    if(coreid>cpupara->getCoreCount()){
        std::cout<<"\"正在执行区\"行数错误！"<<std::endl;
        exit(1);
    }
    QTableWidgetItem* item;
    int i=1;
    QString qstr;
    do{
       item = processingTable->item(coreid-1, i);
        if(!item){
           std::cout<<"\"正在执行区\"出现行空指针"<<std::endl;
            exit(1);
       }
       switch(i){
        case 1:
            qstr=process->name;
            break;
        case 2:
            qstr=QString::number(process->priority);
            break;
        default:
            qstr=QString::number(process->needTime);
            break;
       }
        item->setText(qstr);
       ++i;
    }while(i<=3);
    isProcessingTableClean=false;
}
void ProcessManager::clearProcessingTable(){
    if(isProcessingTableClean) return;
    QTableWidgetItem* item;
    for(int i=0;i<cpupara->getCoreCount();++i){
       for(int j=1;j<4;++j){
            item = processingTable->item(i, j);
            if(item)item->setText("");
       }
    }
    isProcessingTableClean=true;
}
void ProcessManager::clearProcessTable(int rownum){
    int num=(rownum==-1)?INT_MAX:rownum;
    QTableWidgetItem* item;
    for(int i=0;i<num;++i){
       for(int j=0;j<4;++j){
            item = processTable->item(i, j);
            if(item)item->setText("");
            else goto breaklabel;
       }
    }
 breaklabel:{};
}
void ProcessManager::invokeFunc(){
    counting=false;
    switch (funcCode) {
    case 1:
        reloadReady();
        break;
    case 2:
        serveProcess();
        break;
    case 3:
        processingBackToReady();
        break;
    default:
        break;
    }
}
void ProcessManager::kickOnNext(const int&code,const int&interval){
    funcCode=code;
    timer->setSingleShot(true); // 设置定时器为一次性定时器
    timer->start(interval);
}
void ProcessManager::addRows(const int& num){
    if(num<1)return;
    proc*item;
    for(int i=1;i<=num;++i){
        item=readArow(i);
        if(!item)continue;
        registerAProcess(item);
        backqueue.push(item);
    }
    backcon->updateByQueue(backqueue);
    clearProcessTable(num);
}
void ProcessManager::readRowsFromFile(){
    // 打开记事本等文本编辑器
    QString filePath = QFileDialog::getOpenFileName(nullptr,"选择CSV文件", "", "CSV Files (*.csv)");
    std::vector<proc*>tmp;
    readfile(filePath.toStdString(),tmp,this,this->checkPara);
    for(auto& it:tmp){
        registerAProcess(it);
        backqueue.push(std::move(it));
    }
    backcon->updateByQueue(backqueue);
}
proc* ProcessManager::checkPara(const QString&name,const QString&time,const QString&pri,const QString&size){
    if(name==""||processes.count(name)>0)return nullptr;
    bool trans=true; int atime; int apri; int asize;
    atime=time.toInt(&trans);
    asize=size.toInt(&trans);
    apri=pri.toInt(&trans);
    if(!trans||atime<1||asize<1||asize>OSobject::allMemorySize-OSobject::size)return nullptr;
    return new proc(name,atime,apri,asize);
}
//所有的行在QT中是以0开始，这里把第一行的rownum=1
proc* ProcessManager::readArow(const int&rownum){
    if(rownum<1){
        std::cout<<"\"添加执行区\"行数错误！"<<std::endl;
        exit(1);
    }
    QTableWidgetItem* item;
    QString name;QString priority;QString time;QString size;
    int i=0;
    do{
        item = processTable->item(rownum-1, i);
        if(!item){
            std::cout<<"\"添加执行区\"出现行空指针"<<std::endl;
            exit(1);
        }
        switch(i){
        case 0:
            name=item->text();
            break;
        case 1:
            time=item->text();
            break;
        case 2:
            priority=item->text();
            break;
        default:
            size=item->text();
            break;
        }
        ++i;
    }while(i<4);
    return checkPara(name,time,priority,size);
}
void ProcessManager::addPreSuspendSet(const QString&name){
    proc*process=processes[name];
    if(!process){
        std::cout<<"[严重] [where]:ProcessManager::addPreSuspendSet 选中的process哈希表中不存在！已主动结束"<<std::endl;
        exit(1);
    }
    preSuspendSet.insert(process);
}
void ProcessManager::removeFromPreSuspendSet(const QString&name){
    proc*process=processes[name];
    if(!process){
        std::cout<<"[严重] [where]:PProcessManager::removeFromPreSuspendSet 选中的process哈希表中不存在！"<<std::endl;
        exit(1);
    }
    auto it =preSuspendSet.find(process);
    if(it==preSuspendSet.end()){
        std::cout<<"[严重] [where]:PProcessManager::removeFromPreSuspendSet 试图解除一个 选择挂起队列 中不存在的进程！"<<std::endl;
        exit(1);
    }
    preSuspendSet.erase(it);
}
void ProcessManager::addPreReleaseSet(const QString&name){
    proc*process=processes[name];
    if(!process){
        std::cout<<"[严重] [where]:ProcessManager::addPreReleaseSet 选中的process哈希表中不存在！已主动结束"<<std::endl;
        exit(1);
    }
    preReleaseSet.insert(process);
}
void ProcessManager::removeFromPreReleaseSet(const QString&name){
    proc*process=processes[name];
    if(!process){
        std::cout<<"[严重] [where]:ProcessManager::removeFromPreReleaseSet 选中的process哈希表中不存在！"<<std::endl;
        exit(1);
    }
    auto it =preReleaseSet.find(process);
    if(it==preReleaseSet.end()){
        std::cout<<"[严重] [where]:PProcessManager::removeFromPreSuspendSet 试图解除一个 选择挂起队列 中不存在的进程！"<<std::endl;
        exit(1);
    }
    preReleaseSet.erase(it);
}
void ProcessManager::addSuspendQueue(){
    //当预备挂起队列已经被证实添加到挂起队列，但是考虑到我们更新card的策略是，能修改就不删除，所以有必要将队列一切仍然可能处于clicked的组件，设置为非click
    readycon->setAllNotClicked();
    for(auto it: preSuspendSet){
        suspendqueue.push_back(it);
        readyqueue.remove(it);
        auto iter=std::find(servingqueue.begin(),servingqueue.end(),it);
        if(iter!=servingqueue.end())servingqueue.erase(iter);
        memoryManager->recycleForAProcess(it);
    }
    preSuspendSet.clear();  // 清空 servingqueue
    readycon->updateByQueue(readyqueue);
    suspendcon->updateByQueue(suspendqueue);
}
void ProcessManager::handleRelease(){
    //当预备挂起队列已经被证实添加到挂起队列，但是考虑到我们更新card的策略是，能修改就不删除，所以有必要将队列一切仍然可能处于clicke的组件，设置为非click
    suspendcon->setAllNotClicked();
    proc*procit;
    std::deque<proc*>readyinner;
    std::deque<proc*>::iterator sustmpend=suspendqueue.end();
    std::set<proc*>::iterator pretmpend=preReleaseSet.end();
    bool readyInnerModified=false;
    if(releaseCode==-1){
        for(auto it=suspendqueue.begin();it!=suspendqueue.end();++it){
            while(!memoryManager->allocateForAProcess(*it)){
                if(readyqueue.empty()){
                    std::advance(it,1);
                    sustmpend=it;
                    goto label1;
                }
                procit=readyqueue.get_and_pop_back_without_makeheap();
                readyInnerModified=true;
                memoryManager->recycleForAProcess(procit);
                swapqueue.insert(procit);
            }
            readyqueue.push(*it);
        }
    label1:
        suspendqueue.erase(suspendqueue.begin(),sustmpend);
    }else{
        for(auto it=preReleaseSet.begin();it!=preReleaseSet.end();++it){
            while(!memoryManager->allocateForAProcess(*it)){
                if(readyqueue.empty()){
                    std::advance(it,1);
                    pretmpend=it;
                    goto label2;
                }
                procit=readyqueue.get_and_pop_back_without_makeheap();
                readyInnerModified=true;
                memoryManager->recycleForAProcess(procit);
                swapqueue.insert(procit);
            }
            readyqueue.push(*it);
            auto iter=std::find(suspendqueue.begin(),suspendqueue.end(),*it);
            if(iter==suspendqueue.end()){
                std::cout<<"[严重] [where]:ProcessManager::releaseFromSuspendQueue 试图解除一个 选择释放挂起队列 中不存在的进程！"<<std::endl;
                exit(1);
            }
            suspendqueue.erase(iter);
        }
    label2:
        preReleaseSet.erase(preReleaseSet.begin(),pretmpend);
    }
    if(readyInnerModified)readyqueue.adjust_heap();
    suspendcon->updateByQueue(suspendqueue);
    swapcon->updateByQueue(swapqueue);
    //这些每个函数就不单独提供更新gui的操作了，因为他们是在serveProcess函数中被调用，并且serveProcess调用很多函数，想要做到 不必要的刷新操作 都不要，有些困难，可能要定义很多bool标志，所以索性每次都统一刷新一次。
}
void ProcessManager::addSuspendQueueFromProcessing(){
    // 获取用户选择的行
    QList<QTableWidgetItem*> selectedItems = processingTable->selectedItems();
    //我这不是多此一举，只是selectedItems()无论你选中几行，都会返回 行数*选中数 个元素，欣慰的是，有重复但没有错失，但是要过滤重复。
    QSet<int> selectedRows;
    // 获取所有选中行的索引
    foreach (QTableWidgetItem* item, selectedItems)selectedRows.insert(item->row());
    proc*p=nullptr;
    foreach(const int&row,selectedRows){
        p=processes[processingTable->item(row,1)->text()];
        if(!p) return;
        ++(p->needTime);
        suspendqueue.push_back(p);
        auto iter=std::find(servingqueue.begin(),servingqueue.end(),p);
        if(iter!=servingqueue.end())servingqueue.erase(iter);
        memoryManager->recycleForAProcess(p);
        for(int i=1;i<4;++i){
            processingTable->item(row,i)->setText("");
        }
    }
    suspendcon->updateByQueue(suspendqueue);
    processingTable->clearSelection();
}
void ProcessManager::releaseFromSuspendQueue(const int&code){
    releaseCode=code;
    if(!reloadedJustNow){
        needHandleRelease=true;
        timer->stop();
        serveProcess();
    }else{
        needHandleRelease=false;
        timer->stop();
        handleRelease();
        readycon->updateByQueue(readyqueue);
        kickOnNext(2,serveTime);
    }
}
void ProcessManager::addDetailInfo(proc*process){
    MyItem*item=new MyItem(process);
    ((QStandardItemModel*)(detailList->model()))->appendRow(item);
}
void ProcessManager::clearDetailInfo(){
    ((QStandardItemModel*)(detailList->model()))->clear();
}
void ProcessManager::revertToOrigin(){
    timer->stop();
    processes.clear();

    backqueue.clear();
    readyqueue.clear();
    suspendqueue.clear();
    servingqueue.clear();
    swapqueue.clear();

    backcon->clear();
    readycon->clear();
    suspendcon->clear();
    swapcon->clear();

    preReleaseSet.clear();
    preSuspendSet.clear();

    clearProcessTable();
    clearProcessingTable();
    clearDetailInfo();
    memoryManager->revertToOrigin();

    counting=false;//是否还在计时
    isProcessingTableClean=true;
    funcCode=-1;
    reloadedJustNow=false;
    reloadedFromBackJustNow=false;
    needHandleRelease=false;
    releaseCode=0;
    initBackQueue();
}
// funcCode 1.开始初始化就绪队列2.开始不断拿取进程到CPU
void ProcessManager::alertReleaseFailure(const std::vector<proc*>& alist){
    msgBox.setText("由于内存原因，以下进程暂时无法分配内存");
    QString tip;
    for(proc* const& it:alist){
        tip.append(it->name);
        tip.append('\n');
    }
    msgBox.setInformativeText(tip);
    msgBox.setIcon(QMessageBox::Information);
    msgBox.exec();
}
