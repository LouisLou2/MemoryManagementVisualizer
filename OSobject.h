#ifndef OSOBJECT_H
#define OSOBJECT_H
class OSobject{
public:
    static int memoryStart;
    static int size;
    static int allMemorySize;

    static int memoryForUserStart;
    static int memoryForUserSize;
    static void ChangeAllMemSize(const int&size);
};
#endif // OSOBJECT_H
