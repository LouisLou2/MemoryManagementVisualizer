#ifndef MYTIMER_H
#define MYTIMER_H
#include<QTimer>
#include <QDateTime>
class myTimer:public QObject
{
    Q_OBJECT
public:
    myTimer(QObject*parent=nullptr);
    void start(const int&interval=-1);
    void setInterval(const int&interval);
    void continueCounting();
    void setSingleShot(bool single);
    void stop();
    ~myTimer();
signals:
    void timeout();
private slots:
    void getInnerTimeOut();
private:
    QTimer*timer;
    bool running;
    bool single;
    int remaining;
    int interval;
    QDateTime exStartTime;
};

#endif // MYTIMER_H
