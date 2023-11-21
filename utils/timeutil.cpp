#include "timeutil.h"
#include <chrono>
#include <QString>
QString getNowTimeStr(){
    // 获取当前系统时间点
    auto now = std::chrono::system_clock::now();

    // 将时间点转换为时间结构体
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);

    struct tm timeInfo;
    localtime_s(&timeInfo, &currentTime);  // 使用 localtime_s 函数

    // 使用时间结构体格式化时间字符串（只包含时分秒）
    char timeString[9];  // 时分秒的格式为 "HH:MM:SS"
    std::strftime(timeString, sizeof(timeString), "%H:%M:%S", &timeInfo);

    return QString::fromLatin1(timeString);
}
