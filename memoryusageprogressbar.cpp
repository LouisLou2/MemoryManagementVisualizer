#include "memoryusageprogressbar.h"
#include <QVBoxLayout>
MemoryUsageProgressBar::MemoryUsageProgressBar(QWidget *parent) : QWidget(parent) {
    progressBar = new QProgressBar(this);
    progressBar->setRange(0, 100);
    progressBar->setValue(50); // 设置一个固定的值
    //setFixedSize(300, 400); // 设置窗口固定大小
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(progressBar);
    setLayout(layout);
}

void MemoryUsageProgressBar::updateMemoryUsage(const double& per) {
    progressBar->setValue(per*100);
}
