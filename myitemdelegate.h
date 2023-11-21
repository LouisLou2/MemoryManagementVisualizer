#ifndef MYITEMDELEGATE_H
#define MYITEMDELEGATE_H
#include <QStyledItemDelegate>
#include <QPen>
#include <QPainter>
class MyItemDelegate : public QStyledItemDelegate {
public:
    MyItemDelegate(QWidget*parent=nullptr);
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
};
#endif // MYITEMDELEGATE_H
