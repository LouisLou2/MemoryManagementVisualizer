#ifndef SCROLLABLECARDCONTAINER_H
#define SCROLLABLECARDCONTAINER_H
#include<QWidget>
#include<QVBoxLayout>
#include <queue>
#include <set>
#include "cardwidget.h"
#include "derivedpriorityqueue.h"
class ScrollableCardContainer : public QWidget
{
    Q_OBJECT
    public:
        ScrollableCardContainer(QWidget* parent = nullptr);
        ~ScrollableCardContainer();
        void addRow(proc* cardinfo);
        void resizeOfRemoval();
        void pop();
        void remove(const int&index);
        CardWidget*getTop();
        bool empty()const;
        int size()const;
        void updateByQueue(DerivedPriorityQueue<proc*,std::deque<proc*>,CompareProc>&thequeue);
        void updateByQueue(std::deque<proc*>&thequeue);
        void updateByQueue(std::set<proc*>&thequeue);
        void removeClickedCard();
        void setAllNotClicked();
        void eraseIfClicked(const QString&name);
        void updateClickedState(const std::set<proc*>&theset);
        void clear();
    signals:
        void CardClicked(const QString&name);
        void BreakCardClicked(const QString&name);
    private:
        int count=0;
        int width=300;
        int cardHeight=90;
        QVBoxLayout*scrollLayout;
        void UpdateAndAdjustSize();
};
#endif // SCROLLABLECARDCONTAINER_H
