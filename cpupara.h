#ifndef CPUPARA_H
#define CPUPARA_H
#include <string>
class CPUPara{
public:
    // 构造函数
    CPUPara(int id, const std::string& model, int coreCount, double clockSpeed,int acpuParallelThreads)
        : id(id), model(model),clockSpeed(clockSpeed){
        if(coreCount>=1)this->coreCount=coreCount;
        else{
            std::cout<<"[严重]where: CPUPara constructor coreCount传入错误值"<<std::endl;
        }
        if(acpuParallelThreads>=1&&acpuParallelThreads>=coreCount)this->cpuParallelThreads=acpuParallelThreads;
        else{
            std::cout<<"[严重]where: CPUPara constructor acpuParallelThreads传入错误值"<<std::endl;
        }
    }
    CPUPara(int coreCount):CPUPara(123,"NormalModel",coreCount,12,INT_MAX){}
    CPUPara(int coreCount,int acpuParallelThreads):CPUPara(123,"NormalModel",coreCount,12, acpuParallelThreads){}
    // 获取 CPU ID
    int getID() const {
        return id;
    }

    // 获取 CPU 型号
    const std::string& getModel() const {
        return model;
    }

    // 获取核心数量
    int getCoreCount() const {
        return coreCount;
    }

    // 获取时钟速度
    double getClockSpeed() const {
        return clockSpeed;
    }
    int getcpuParallelThreads(){
        return cpuParallelThreads;
    }
    // 设置 CPU ID
    void setID(int newID) {
        id = newID;
    }

    // 设置 CPU 型号
    void setModel(const std::string& newModel) {
        model = newModel;
    }

    // 设置核心数量
    void setCoreCount(int newCoreCount) {
        coreCount = newCoreCount;
    }

    // 设置时钟速度
    void setClockSpeed(double newClockSpeed) {
        clockSpeed = newClockSpeed;
    }
    void setcpuParallelThreads(const int&num){
        cpuParallelThreads=num;
    }

private:
    int id;            // CPU 编号
    std::string model; // CPU 型号
    int coreCount;     // 核心数量
    double clockSpeed; // 时钟速度
    int cpuParallelThreads=INT_MAX;
};

#endif // CPUPARA_H
