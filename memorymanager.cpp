#include "memorymanager.h"
#include "proc.h"
MemoryManager::MemoryManager(QObject*parent):QObject(parent)
{
    initManager();
}
MemoryManager::MemoryManager(QLabel*allmemLabel,QLabel*memUsingLabel,QLabel*memLeftLabel,MemoryUsageProgressBar*membar,MemoryWidget*memwidget,QTableWidget*memDetailTable,QObject*parent):QObject(parent){
    initManager();
    memguiManager=new MemoryGuiManager(allmemLabel,memUsingLabel,memLeftLabel,membar,memwidget,memDetailTable,this);
    //memguimanager内部的memWidget的内部的memBlocks指针与此类一致，这样更新起来方便;
    memguiManager->setProcList(memSet);
    memguiManager->Update_Memory_GUI(usingSize);
    connect(this,&MemoryManager::MemoryWidgetNeedUpdate,this->memguiManager,&MemoryGuiManager::Update_Memory_GUI);
}
void MemoryManager::initManager(){
    memSet=new std::set<proc*,CompareByMemStart>();
    proc*osproc=new proc("OS",0,0,OSobject::size);
    osproc->memoryStart=OSobject::memoryStart;
    memSet->insert(osproc);
    freeMemList=new std::list<MemoryBlock*>();
    MemoryBlock*mem_excp_os=new MemoryBlock(OSobject::memoryStart+OSobject::size,OSobject::memoryForUserSize);
    freeMemList->push_back(mem_excp_os);
    usingSize=OSobject::size;
    allocateFunc=&MemoryManager::FirstFit;
}
void MemoryManager::changeAllocateAlgo(int index){
    switch(index){
    case 0:
        allocateFunc=&MemoryManager::FirstFit;
        break;
    case 1:
        allocateFunc=&MemoryManager::BestFit;
        break;
    default:
        allocateFunc=&MemoryManager::FirstFit;
        break;
    }
}
bool MemoryManager::FirstFit(proc*process){
    for(auto freeblock=freeMemList->begin();freeblock!=freeMemList->end();++freeblock){
        if((*freeblock)->size<process->size)continue;
        process->memoryStart=(*freeblock)->start;
        memSet->insert(process);
        if((*freeblock)->size>process->size){
            (*freeblock)->start+=process->size;
            (*freeblock)->size-=process->size;
        }else{
            freeMemList->erase(freeblock);
        }
        usingSize+=process->size;
        return true;
    }
    return false;
}
bool MemoryManager::BestFit(proc*process){
    std::list<MemoryBlock*>::iterator bestIter=freeMemList->end();
    int blocksize=INT_MAX;
    for(auto freeblock=freeMemList->begin();freeblock!=freeMemList->end();++freeblock){
        if((*freeblock)->size<process->size)continue;
        if((*freeblock)->size<blocksize){
            blocksize=(*freeblock)->size;
            bestIter=freeblock;
        }
    }
    if(bestIter==freeMemList->end())return false;
    process->memoryStart=(*bestIter)->start;
    memSet->insert(process);
    if((*bestIter)->size>process->size){
        (*bestIter)->start+=process->size;
        (*bestIter)->size-=process->size;
    }else{
        //这里其实就是==的情况
        freeMemList->erase(bestIter);
    }
    usingSize+=process->size;
    return true;
}
bool MemoryManager::allocateForAProcess(proc*process){
    if((this->*allocateFunc)(process)){
        emit MemoryWidgetNeedUpdate(usingSize);
        return true;
    }
    return false;
}
void MemoryManager::recycleForAProcess(proc*process){
    auto freeblock=freeMemList->begin();
    bool Inbetween=true;
    while(freeblock!=freeMemList->end()){
        if((*freeblock)->start>process->getEnd())break;
        if((*freeblock)->getEnd()==process->memoryStart){
            Inbetween=false;
            auto nextIt=freeblock;
            std::advance(nextIt,1);
            if(nextIt!=freeMemList->end()&&process->getEnd()==(*nextIt)->start){
                (*freeblock)->size+=process->size;
                (*freeblock)->size+=(*nextIt)->size;
                freeMemList->erase(nextIt);
                break;
            }
            (*freeblock)->size+=process->size;
            break;
        }
        if(process->getEnd()==(*freeblock)->start){
            (*freeblock)->start-=process->size;
            (*freeblock)->size+=process->size;
            Inbetween=false;
            break;
        }
        std::advance(freeblock,1);
    }
    if(Inbetween)freeMemList->insert(freeblock,new MemoryBlock(process->memoryStart,process->size));
    memSet->erase(process);
    usingSize-=process->size;
    emit MemoryWidgetNeedUpdate(usingSize);
}
void MemoryManager::revertToOrigin(){
    memSet->clear();
    proc*osproc=new proc("OS",0,0,OSobject::size);
    osproc->memoryStart=OSobject::memoryStart;
    memSet->insert(osproc);
    freeMemList->clear();
    MemoryBlock*mem_excp_os=new MemoryBlock(OSobject::memoryStart+OSobject::size,OSobject::memoryForUserSize);
    freeMemList->push_back(mem_excp_os);
    usingSize=OSobject::size;
    //务必弄完以上的再来set下面的，因为他们类内引用同一个memSet指针，上面的更改妥帖之后，更新下面的，自然水到渠成。

    memguiManager->setProcList(memSet);
    memguiManager->Update_Memory_GUI(usingSize);
}
void MemoryManager::changeAllMemSize(){
    freeMemList->clear();
    MemoryBlock*mem_excp_os=new MemoryBlock(OSobject::memoryStart+OSobject::size,OSobject::memoryForUserSize);
    freeMemList->push_back(mem_excp_os);
    memguiManager->Update_Memory_GUI(usingSize);
}
