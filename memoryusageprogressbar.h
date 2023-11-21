#ifndef MEMORYUSAGEPROGRESSBAR_H
#define MEMORYUSAGEPROGRESSBAR_H

#include <QWidget>
#include <QProgressBar>
#include <QTimer>
#include <random>

class MemoryUsageProgressBar : public QWidget {
    Q_OBJECT

public:
    explicit MemoryUsageProgressBar(QWidget *parent = nullptr);
    void updateMemoryUsage(const double& per);
private:
    QProgressBar *progressBar;
};
#endif // MEMORYUSAGEPROGRESSBAR_H
