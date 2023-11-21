#include "parseutil.h"
#include<sstream>
#include<fstream>
#include<QString>

QString units[4]={
    "Kb","Mb","Gb","Tb"
};

void splitString(const std::string& str, const char& delimeter,std::string*tokens) {
    std::string line;
    std::stringstream ss(str);
    int i = 0;
    while (getline(ss, line, delimeter)) {
        tokens[i++] = line;
    }
}
void splitStringDeliStr (const std::string& input, const std::string& delimiter, std::string* tokens) {
    size_t startPos = 0;
    size_t endPos = input.find(delimiter);
    int i = 0;
    while (endPos != std::string::npos) {
        tokens[i++]=input.substr(startPos, endPos - startPos);
        startPos = endPos + delimiter.length();
        endPos = input.find(delimiter, startPos);
    }
    tokens[i]=input.substr(startPos, endPos);
}
void readfile(const std::string& filename,std::vector<proc*>&pq,ProcessManager*manager,proc* (ProcessManager::*condition)(const QString&name,const QString&time,const QString&pri,const QString&size)) {
    std::ifstream in(filename);
    if (!in.is_open()) {
        std::cout << "文件打开失败" << std::endl;
        return;
    }
    bool needCheck=false;
    if(manager&&condition)needCheck=true;
    std::string tokens[4];
    std::string line;
    char delimeter = ',';
    getline(in, line);//第一行标头忽略
    while (getline(in, line)) {
        splitString(line, delimeter, tokens);
        proc*process=nullptr;
        if(needCheck){
            process=(manager->*condition)(QString::fromStdString(tokens[0]),QString::fromStdString(tokens[1]),QString::fromStdString(tokens[2]),QString::fromStdString(tokens[3]));
        }else{
            process=new proc(QString::fromStdString(tokens[0]),stoi(tokens[1]),stoi(tokens[2]),stoi(tokens[3]));
        }
        if(process)pq.push_back(process);
    }
}
QString getSizeStr(int size){
    double dsize=static_cast<double>(size);
    int i=0;
    while(dsize>1024){
        dsize/=1024;
        ++i;
    }
    QString res=QString::number(dsize,'d',2);
    res.append(" ");
    res.append(units[i]);
    return res;
}
