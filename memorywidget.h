#ifndef MEMORYWIDGET_H
#define MEMORYWIDGET_H
#include<QWidget>
#include"proc.h"
#include"OSobject.h"
#include <set>
class MemoryWidget : public QWidget {
public:
    MemoryWidget(QWidget *parent = nullptr);
    MemoryWidget(std::set<proc*,CompareByMemStart>*& blocks, QWidget *parent = nullptr);
    void setMemBlocks(std::set<proc*,CompareByMemStart>*& blocks);
    void setTotalMemorySize(int size);
    void revertToOrigin();
public slots:
    void updateTheWidget();
protected:
    void paintEvent(QPaintEvent *event) override;
private:
    std::set<proc*,CompareByMemStart>* procs_;
    QColor getBlockColor(const bool&isOS);
    QColor color_nonfree=QColor(Qt::blue);
    QColor color_free=QColor(Qt::white);
    QColor color_os=QColor(Qt::yellow);
    int totalMemorySize=OSobject::allMemorySize;
};
#endif // MEMORYWIDGET_H
