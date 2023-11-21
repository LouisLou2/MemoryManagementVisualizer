#include"ScrollableCardContainer.h"
#include<QVBoxLayout>
#include<QScrollArea>
#include <QListView>
#include <QStandardItemModel>
#include <QPushButton>
#include"cardwidget.h"

ScrollableCardContainer::ScrollableCardContainer(QWidget* parent)
        : QWidget(parent) {

    QVBoxLayout* layout = new QVBoxLayout(this);

    // 创建QScrollArea
    QScrollArea* scrollArea = new QScrollArea(this);
    QWidget* scrollWidget = new QWidget(this);

    // 设置QScrollArea的内容
    scrollLayout = new QVBoxLayout(this);
    scrollWidget->setLayout(scrollLayout);
    scrollArea->setWidget(scrollWidget);
    // 将QScrollArea添加到主界面
    layout->addWidget(scrollArea);
    setLayout(layout);
}
ScrollableCardContainer::~ScrollableCardContainer(){
    clear();
}
void ScrollableCardContainer::addRow(proc* cardinfo){
    ++count;
    CardWidget*card=new CardWidget(*cardinfo);
    connect(card,&CardWidget::SelfClicked,this,&ScrollableCardContainer::CardClicked);
    connect(card,&CardWidget::BreakSelfClicked,this,&ScrollableCardContainer::BreakCardClicked);
    scrollLayout->addWidget(card);
    UpdateAndAdjustSize();
}
void ScrollableCardContainer::pop(){
    remove(0);
}
void ScrollableCardContainer::remove(const int&index){
    QLayoutItem* item = scrollLayout->itemAt(index); // 获取位于顶部的子部件
    if(!item){
        std::cout<<"[警告]企图从空容器中删除！"<<std::endl;
        return;
    }
    CardWidget* awidget =(CardWidget*)(item->widget()); // 获取子部件
    --count;
    scrollLayout->removeWidget(awidget); // 从布局中移除
    delete awidget;
    UpdateAndAdjustSize();
    std::cout<<scrollLayout->count()<<std::endl;
}
void ScrollableCardContainer::resizeOfRemoval(){
    QLayoutItem* topItem = scrollLayout->itemAt(0); // 获取位于顶部的子部件
    if(!topItem){
        std::cout<<"[警告]企图在空容器中缩减大小！"<<std::endl;
        return;
    }
    --count;
    UpdateAndAdjustSize();
}
CardWidget* ScrollableCardContainer::getTop(){
    QLayoutItem* topItem = scrollLayout->itemAt(0); // 获取位于顶部的子部件
    QWidget* topWidget =nullptr;
    if (topItem) {
       topWidget = topItem->widget(); // 获取子部件
    }
    return (CardWidget*)topWidget;
}
bool ScrollableCardContainer::empty()const{
    return scrollLayout->isEmpty();
}
int ScrollableCardContainer::size()const{
    return scrollLayout->count();
}
void ScrollableCardContainer::updateByQueue(DerivedPriorityQueue<proc*,std::deque<proc*>,CompareProc>&thequeue){
    int index=0;
    //注意优先队列实质是堆，所以只能保证堆顶元素是最值，所以不能靠获取优先队列内部元素，来寻找第几小（大）的元素。
    DerivedPriorityQueue<proc*,std::deque<proc*>,CompareProc>tmpqueue=thequeue;
    while(!tmpqueue.empty()){
       QLayoutItem* item = scrollLayout->itemAt(index);
       if(!item){
           addRow(tmpqueue.top());
       }else{
           CardWidget*card=(CardWidget*)item->widget();
           card->updateByProc(*(tmpqueue.top()));
       }
       ++index;
       tmpqueue.pop();
    }
    while(index<count){
       remove(index);
    }
    UpdateAndAdjustSize();
}
void ScrollableCardContainer::updateByQueue(std::deque<proc*>&thequeue){
    int index=0;
    while(index<thequeue.size()){
       QLayoutItem* item = scrollLayout->itemAt(index);
       if(!item){
            addRow(thequeue[index]);
       }else{
            CardWidget*card=(CardWidget*)item->widget();
            card->updateByProc(*(thequeue[index]));
       }
       ++index;
    }
    while(index<count){
       remove(index);
    }
    UpdateAndAdjustSize();
}
void ScrollableCardContainer::updateByQueue(std::set<proc*>&thequeue){
    int index=0;
    for(const auto&it:thequeue){
       QLayoutItem* item = scrollLayout->itemAt(index);
       if(!item){
            addRow(it);
       }else{
            CardWidget*card=(CardWidget*)item->widget();
            card->updateByProc(*it);
       }
       ++index;
    }
    while(index<count){
       remove(index);
    }
    UpdateAndAdjustSize();
}
void ScrollableCardContainer::UpdateAndAdjustSize(){
    scrollLayout->update(); // 更新布局
    scrollLayout->parentWidget()->resize(width,cardHeight*count);
}
//这个函数貌似有点问题，调用后还是会有的被选中
void ScrollableCardContainer::removeClickedCard(){
    QLayoutItem* item = scrollLayout->itemAt(0);
    CardWidget*card;
    int index=0;
    while(item){
       card=(CardWidget*)item->widget();
       if(card->isClicked())scrollLayout->removeWidget(card);
       delete card;
       --count;
       UpdateAndAdjustSize();
       item = scrollLayout->itemAt(++index);
    }
}
void ScrollableCardContainer::setAllNotClicked(){
    QLayoutItem* item = scrollLayout->itemAt(0);
    CardWidget*card;
    int index=0;
    while(item){
       card=(CardWidget*)item->widget();
       card->eraseClicked();
       item = scrollLayout->itemAt(++index);
    }
}
void ScrollableCardContainer::eraseIfClicked(const QString&name){
    QLayoutItem* item = scrollLayout->itemAt(0);
    CardWidget*card;
    int index=0;
    while(item){
       card=(CardWidget*)item->widget();
       if(card->getName()==name&&card->isClicked())card->eraseClicked();
       item = scrollLayout->itemAt(++index);
    }
}
void ScrollableCardContainer::updateClickedState(const std::set<proc*>&theset){
    //参函数复杂度是O(mn),不很理想，也许可以通过其它策略优化一下，现在先不考虑
    QLayoutItem* item = scrollLayout->itemAt(0);
    QString name;
    CardWidget*card=nullptr;
    int index=0;
    bool res=false;
    while(item){
       card=(CardWidget*)item->widget();
       name=card->getName();
       for(auto it:theset){
            if(it->name==name){
                res=true;
                break;
            }
       }
       if(res)card->setClicked();
       else card->eraseClicked();
       res=false;
       item = scrollLayout->itemAt(++index);
    }
}
void ScrollableCardContainer::clear(){
    while(count>0){
       remove(0);
    }
}
