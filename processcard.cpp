#include "processcard.h"
#include <QVBoxLayout>
#include <QPainter>

ProcessCard::ProcessCard(const QString& name, int totalTime, const QString& creationTime, int priority, const QString& status, QWidget* parent)
    : QWidget(parent) {

    QVBoxLayout* layout = new QVBoxLayout;
    nameLabel = new QLabel("进程名: " + name);
    totalTimeLabel = new QLabel("总执行时间: " + QString::number(totalTime));
    creationTimeLabel = new QLabel("创建时间: " + creationTime);
    priorityLabel = new QLabel("优先级: " + QString::number(priority));
    statusLabel = new QLabel("状态: " + status);
    QFont font("Microsoft YaHei", 10);

    // 设置标签的字体
    nameLabel->setFont(font);
    totalTimeLabel->setFont(font);
    creationTimeLabel->setFont(font);
    priorityLabel->setFont(font);
    statusLabel->setFont(font);

    layout->addWidget(nameLabel);
    layout->addWidget(totalTimeLabel);
    layout->addWidget(creationTimeLabel);
    layout->addWidget(priorityLabel);
    layout->addWidget(statusLabel);

    setLayout(layout);
}
void ProcessCard::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.fillRect(rect(), QColor(240, 239, 239)); // 设置背景颜色
    painter.drawRect(rect());
    //painter.drawText(rect(), Qt::AlignCenter, "Custom Card Background");
    // 其他绘制操作，例如绘制文本、边框等
}

ProcessCard::ProcessCard(){
    ProcessCard("aprocess", 67, "aprocess", 6, "const QString& status");
}
