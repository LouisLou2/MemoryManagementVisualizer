#ifndef MEMORYGUIMANAGER_H
#define MEMORYGUIMANAGER_H
#include <QLabel>
#include <QTableWidget>
#include "memoryusageprogressbar.h"
#include "memorywidget.h"
class MemoryGuiManager: public QObject
{
    Q_OBJECT
public:
    MemoryGuiManager(QLabel*allmemLabel,QLabel*memUsingLabel,QLabel*memLeftLabel,MemoryUsageProgressBar*membar,MemoryWidget*memwidget,QTableWidget*memDetailTable,QObject*parent=nullptr);
    void setProcList(std::set<proc*,CompareByMemStart>* blocks);
public slots:
    void Update_Memory_GUI(const int&usingSize);
private:
    QLabel*allmemLabel;
    QLabel*memUsingLabel;
    QLabel*memLeftLabel;
    MemoryUsageProgressBar*membar;
    MemoryWidget*memwidget;
    QTableWidget*memDetailTable;
    std::set<proc*,CompareByMemStart>* procs;
    void updateTable();
};

#endif // MEMORYGUIMANAGER_H
