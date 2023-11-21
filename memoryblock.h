#ifndef MEMORYBLOCK_H
#define MEMORYBLOCK_H


struct MemoryBlock
{
    int start;
    int size;
    MemoryBlock();
    MemoryBlock(const int&astart,const int&asize);
    int getEnd();
};

#endif // MEMORYBLOCK_H
