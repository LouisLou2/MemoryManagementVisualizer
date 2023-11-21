#ifndef CARDWIDGET_H
#define CARDWIDGET_H
#include<QString>
#include<QWidget>
#include <iostream>
#include "proc.h"
class CardWidget : public QWidget {
    Q_OBJECT
public:
    //此函数适合进程首次创建时
    CardWidget(const QString&name,
               const int&priority,
               const int&time,
               const int&size,
               QWidget* parent = nullptr);


    CardWidget(const QString&name,
               const QString&createdTime,
               const int&priority,
               const int&time,
               const int& needTime,
               const int&status,
               const int&size,
               QWidget* parent = nullptr);
    CardWidget(const proc&p,QWidget* parent = nullptr);
    ~CardWidget();
    void setPriority(const int&pri);
    void setNeedTime(const int&need);
    void setStatus(const int&status);
    void updateByProc(const proc&process);
    //getter
    QString getName() const;
    QString getCreatedTime() const;
    int getPriority() const;
    int getTime() const;
    int getNeedTime() const;
    int getStatus() const;
    bool operator<(const CardWidget&c)const;
    bool operator>(const CardWidget&c)const;
    friend std::ostream& operator<<(std::ostream& _cout,const CardWidget&c);
    static proc CardWidgetToProc(CardWidget*card);
    static bool isAccording(const proc&process,const CardWidget&card);
    void eraseClicked();
    bool isClicked();
    void setClicked();
signals:
    void SelfClicked(const QString&name);
    void BreakSelfClicked(const QString&name);
protected:
    void paintEvent(QPaintEvent*) override;
    void mousePressEvent(QMouseEvent *event) override;
private:
    QString name;
    QString createdTime;
    int*priority=new int(0);
    int time;//即总共需要上CPU的次数
    int*needTime=new int(0);//即还需要上CPU的次数
    int*status=new int(0);
    int*size=new int(0);
    QStringList cardStr;
    bool clicked=false;
};

#endif // CARDWIDGET_H
