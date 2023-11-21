#include "OSobject.h"
int OSobject::memoryStart=0;
int OSobject::size=512*1024;
int OSobject::allMemorySize=8*1024*1024;
int OSobject::memoryForUserStart=size+memoryStart;
int OSobject::memoryForUserSize=allMemorySize-size-memoryStart;
void OSobject::ChangeAllMemSize(const int&size){
    OSobject::allMemorySize=size;
    OSobject::memoryForUserSize=OSobject::allMemorySize-OSobject::size-OSobject::memoryStart;
}
