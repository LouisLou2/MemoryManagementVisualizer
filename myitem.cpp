#include "myitem.h"
#include <QString>
MyItem::MyItem(proc*process) {
    //setSizeHint(QSize(10, 130)); // 设置项的大小，可以根据需要进行调整
    setData(QVariant(10), Qt::UserRole); // 设置圆角半径，如果需要的话
    // 设置项的圆角属性
    setData(QColor(255,250,250), Qt::BackgroundRole); // 设置背景颜色
    QString str="";
    str.append('\n');
    str.append("进程名: ");
    str.append(process->name+"\t");
    str.append("  ");
    str.append("优先级: ");
    str.append(QString::number(process->priority));
    str.append("\n\n");
    str.append("运行时间: ");
    str.append(QString::number(process->needTime)+'\t');
    str.append("  ");
    str.append("创建时间: ");
    str.append(process->createdTime);
    str.append("\n\n");
    str.append("内存占用: ");
    str.append(QString::number(process->size));
    setText(str);
    QFont font; font.setBold(true);
    setFont(font);
}
