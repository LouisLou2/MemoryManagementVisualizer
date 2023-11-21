#include "memorywidget.h"
#include "utils/parseutil.h"
#include <QPainter>
MemoryWidget::MemoryWidget(QWidget *parent): QWidget(parent)
{
    //setFixedSize(1000, 250); // 设置窗口大小
    setStyleSheet("border: 8px solid lightblue;"); // 添加边框样式
    procs_=nullptr;
    totalMemorySize=OSobject::allMemorySize;
}
MemoryWidget::MemoryWidget(std::set<proc*,CompareByMemStart>*& blocks, QWidget *parent): QWidget(parent), procs_(blocks)
{
    procs_=blocks;
    totalMemorySize=OSobject::allMemorySize;
    //setFixedSize(1000, 250); // 设置窗口大小
    setStyleSheet("border: 8px solid lightblue;"); // 添加边框样式
}
void MemoryWidget::setMemBlocks(std::set<proc*,CompareByMemStart>* &blocks){
    procs_=blocks;
    update();
}
void MemoryWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);

    // 绘制边框
    painter.drawRect(0, 0, width() - 1, height() - 1);

    if (!procs_) return;
    int startPos;
    int blockSize;

    // 计算每个 proc 在内存分区中的绘制位置和大小
    int yPosition;
    int blockHeight;
    QColor color;
    for (const auto &proc : *procs_) {
        color=getBlockColor(proc->name=="OS");
        startPos = proc->memoryStart;
        blockSize = proc->size;

        // 计算每个 proc 在内存分区中的绘制位置和大小
        yPosition = (static_cast<double>(startPos) / OSobject::allMemorySize) * height();
        blockHeight = (static_cast<double>(blockSize) /OSobject::allMemorySize) * height();
        //int lineheight=(100000.0 / totalMemorySize) * height();
        painter.fillRect(8, yPosition, width() - 16, blockHeight, color);

        //painter.fillRect(8, yPosition, width() - 16, lineheight, Qt::red);

        // 在绘制文本之前设置字体和颜色
        painter.setPen(QColor(Qt::white)); // 设置文本颜色为白色
        painter.setFont(QFont("Arial", 10)); // 设置字体和大小
        painter.setPen(QColor(Qt::white));
        painter.setFont(QFont("Arial", 10));

        // 获取文本的矩形区域
        QRect textRect(8, yPosition, width() - 16, blockHeight);

        // 将文本放置于矩形中央
        painter.drawText(textRect, Qt::AlignCenter, proc->name+"  "+QString::number(proc->memoryStart)+"  "+getSizeStr(proc->size));
    }
}
QColor MemoryWidget::getBlockColor(const bool&isOS){
    return isOS?QColor(255, 165, 0):QColor(Qt::blue);
}
void MemoryWidget::updateTheWidget(){
    update();
}
void MemoryWidget::setTotalMemorySize(int size){
    totalMemorySize=size;
}
void MemoryWidget::revertToOrigin(){
    //此函数暂时搁置，因为revertToOrigin,可以通过setMemBlocks来实现。
}
