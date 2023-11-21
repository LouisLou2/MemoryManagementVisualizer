#ifndef PARSEUTIL_H
#define PARSEUTIL_H
#include<string>
#include<queue>
#include "cardwidget.h"
#include "derivedpriorityqueue.h"
#include"proc.h"
#include "processmanager.h"
void splitString(const std::string& str, const char& delimeter, std::string* tokens);
void splitStringDeliStr(const std::string& input, const std::string& delimiter, std::string* tokens);
void readfile(const std::string& filename,std::vector<proc*>&pq,ProcessManager*manager=nullptr,proc* (ProcessManager::*condition)(const QString&name,const QString&time,const QString&pri,const QString&size)=nullptr);
QString getSizeStr(int size);
#endif // PARSEUTIL_H
