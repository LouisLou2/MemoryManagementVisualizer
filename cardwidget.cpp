#include "cardwidget.h"
#include "utils/timeutil.h"
#include "processmanager.h"
#include<QPaintEvent>
#include<QPainter>
#include<QString>
CardWidget::CardWidget(const QString&name,
           const int&priority,
                       const int&time,const int&size,QWidget* parent):QWidget(parent){
    CardWidget(name,getNowTimeStr(),priority,time,time,0,size,parent);
}


CardWidget::CardWidget(const QString&name,
           const QString&createdTime,
           const int&priority,
           const int&time,
           const int& needTime,
           const int&status,
           const int&size,
           QWidget* parent):QWidget(parent),name(name),createdTime(createdTime),time(time){
    *(this->priority)=priority;
    *(this->needTime)=needTime;
    *(this->status)=status;
    *(this->size)=size;
    setFixedSize(270, 80);
}
CardWidget::CardWidget(const proc&p,QWidget* parent):CardWidget(p.name,p.createdTime,p.priority,p.time,p.needTime,p.status,p.size,parent){
}
CardWidget::~CardWidget(){
    if(priority)delete priority;
    if(needTime)delete needTime;
    if(status)delete status;
    if(size)delete size;
}
//下面这个函数注意到了painter每次被创建，但是我把他独立出来重用会出错，队列图空白，所以只能这样
void CardWidget::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    // 自定义卡片的背景颜色
    // 根据点击状态设置背景颜色
    if (clicked) {
        painter.setBrush(QColor("lightblue"));
    } else {
        painter.setBrush(QColor(220, 220, 220)); // 默认背景色
    }
    // 创建一个带圆角的矩形
    QRectF roundedRect = rect().adjusted(1, 1, -1, -1);
    int cornerRadius = 10; // 设置圆角的半径
    // 自定义卡片的边框颜色和宽度
    painter.setPen(QPen(QColor(100, 100, 100), 3)); // 例如，深灰色边框，宽度为3像素
    // 绘制圆角矩形
    painter.drawRoundedRect(roundedRect, cornerRadius, cornerRadius);
    // 自定义文本的颜色和字体
    painter.setPen(QColor(0, 0, 0)); // 例如，黑色文本
    QFont font("Arial", 12); // 例如，Arial字体，12号字号
    painter.setFont(font);
    cardStr.clear();
    cardStr<<"   "<<name<<"    "<<QString::number(*priority)<<"    "<<QString::number(*needTime);
    // 居中绘制文本
    painter.drawText(rect(), Qt::AlignLeft | Qt::AlignVCenter, cardStr.join(""));
}
void CardWidget::mousePressEvent(QMouseEvent *event) {
    clicked=!clicked;
    update(); // 强制刷新以调用paintEvent
    if(clicked)emit SelfClicked(this->name);
    else emit BreakSelfClicked(this->name);
    QWidget::mousePressEvent(event);
}
void CardWidget::eraseClicked(){
    if(!clicked)return;
    clicked=false;
    update(); // 强制刷新以调用paintEvent
}
void CardWidget::setClicked(){
    clicked=true;
    update();
}
void CardWidget::setPriority(const int&pri){
    *(this->priority)=pri;
    update();
}
void CardWidget::setNeedTime(const int&need){
    *(this->needTime)=need;
    update();
}
void CardWidget::setStatus(const int&status){
    *(this->status)=status;
    update();
}
void CardWidget::updateByProc(const proc&process){
    this->name=process.name;
    *(this->priority)=process.priority;
    *(this->needTime)=process.needTime;
    *(this->status)=process.status;
    *(this->size)=process.size;
    this->createdTime=process.createdTime;
    this->time=process.time;
    update();
}
proc CardWidget::CardWidgetToProc(CardWidget*card){
    proc p(card->getName(),card->getCreatedTime(),card->getPriority(),card->getTime(),card->getNeedTime(),card->getStatus());
    return p;
}
bool CardWidget::isAccording(const proc&process,const CardWidget&card){
    return process.name==card.getName();
}
//getter
QString CardWidget::getName() const {
    return name;
}
QString CardWidget::getCreatedTime() const {
    return createdTime;
}
int CardWidget::getPriority() const {
    return *priority;
}
int CardWidget::getTime() const {
    return time;
}
int CardWidget::getNeedTime() const {
    return *needTime;
}
int CardWidget::getStatus() const {
    return *status;
}
bool CardWidget::isClicked(){
    return clicked;
}
bool CardWidget::operator<(const CardWidget&c)const{
    return *(this->priority)>*(c.priority)||((*(this->priority)==*(c.priority))&&(*(this->needTime)<*(c.needTime)));
}
bool CardWidget::operator>(const CardWidget&c)const{
    return *(this->priority)<*(c.priority)||((*(this->priority)==*(c.priority))&&(*(this->needTime)>*(c.needTime)));
}
std::ostream& operator<<(std::ostream& _cout,const CardWidget &c){
    _cout <<"name: "<<c.name.toStdString() <<' '
          <<"createdTime: "<< c.createdTime.toStdString()<<' '
          <<"priority: "<<*(c.priority)<<' '
          <<"time: "<<c.time << ' '
          <<"needTime: "<<*(c.needTime)<<' '
          <<"status: "<<*(c.status);
    return _cout;
}

