#include "memoryblock.h"

MemoryBlock::MemoryBlock()
{
    start=0;
    size=0;
}
MemoryBlock::MemoryBlock(const int&astart,const int&asize){
    start=astart;
    size=asize;
}
int MemoryBlock::getEnd(){
    return start+size;
}
