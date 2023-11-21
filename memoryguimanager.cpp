#include "memoryguimanager.h"
#include "utils/parseutil.h"
MemoryGuiManager::MemoryGuiManager(QLabel*aallmemLabel,QLabel*amemUsingLabel,QLabel*amemLeftLabel,MemoryUsageProgressBar*amembar,MemoryWidget*amemwidget,QTableWidget*amemDetailTable,QObject*parent)
    :QObject(parent),allmemLabel(aallmemLabel),memUsingLabel(amemUsingLabel),memLeftLabel(amemLeftLabel),membar(amembar),memwidget(amemwidget),memDetailTable(amemDetailTable){

}
void MemoryGuiManager::setProcList(std::set<proc*,CompareByMemStart>* blocks){
    procs=blocks;
    memwidget->setMemBlocks(blocks);
}
void MemoryGuiManager::Update_Memory_GUI(const int&usingSize){
    allmemLabel->setText(getSizeStr(OSobject::allMemorySize));
    memLeftLabel->setText(getSizeStr(OSobject::allMemorySize-usingSize));
    memUsingLabel->setText(getSizeStr(usingSize));
    membar->updateMemoryUsage(((double)(usingSize))/OSobject::allMemorySize);
    memwidget->updateTheWidget();
    updateTable();
}
void MemoryGuiManager::updateTable(){
    int rowCount=memDetailTable->rowCount();
    int index=0;
    QTableWidgetItem*space=nullptr;
    for(auto proc:*procs){
        if(index>=rowCount){
            memDetailTable->insertRow(rowCount);
            for(int i=0;i<3;++i){
                space=new QTableWidgetItem("");
                memDetailTable->setItem(index,i,space);
            }
        }
        for(int i=0;i<3;++i){
            space=memDetailTable->item(index,i);
            switch (i) {
            case 0:
                space->setText(proc->name);
                break;
            case 1:
                space->setText(QString::number(proc->memoryStart));
                break;
            default:
                space->setText(QString::number(proc->size));
                break;
            }
        }
        ++index;
    }
    while(index<rowCount){
        //如果有富余的行，别删除了吧，也许下一次还要用，清空内容吧
        memDetailTable->removeRow(--rowCount);
    }
}
