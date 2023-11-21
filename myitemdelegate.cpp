#include "myitemdelegate.h"

void MyItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const{
    QStyledItemDelegate::paint(painter, option, index);

    // 在每个项之后绘制一条分隔线
    int lineHeight = option.rect.bottom();
    painter->setPen(QPen(Qt::gray, 1)); // 线条颜色和宽度
    painter->drawLine(option.rect.topLeft(), option.rect.topRight()); // 从项的顶部左侧到右侧的线
    painter->drawLine(option.rect.bottomLeft(), option.rect.bottomRight()); // 从项的底部左侧到右侧的线
}
MyItemDelegate::MyItemDelegate(QWidget*parent):QStyledItemDelegate(parent){

}
