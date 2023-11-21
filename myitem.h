#ifndef MYITEM_H
#define MYITEM_H
#include <QStandardItem>
#include "proc.h"
class MyItem : public QStandardItem {
public:
    MyItem(proc*process);
};
#endif // MYITEM_H
