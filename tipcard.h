#ifndef TIPCARD_H
#define TIPCARD_H
#include <QApplication>
#include <QWidget>
#include <QToolTip>
#include <QMouseEvent>
#include <QLabel>
#include <QPainter>
class TipCard : public QWidget {
public:
    TipCard(QWidget* parent = nullptr) : QWidget(parent) {
        setMouseTracking(true); // 启用鼠标跟踪以检测鼠标悬停
        setFixedSize(200, 200);
    }
    void paintEvent(QPaintEvent*) override {
        // 创建一个QPainter对象以在QWidget上绘制图形
        QPainter painter(this);

        // 设置绘图属性，例如颜色、画刷、画笔等
        QPen pen(Qt::red); // 使用红色画笔
        painter.setPen(pen);

        QBrush brush(Qt::yellow); // 使用黄色画刷
        painter.setBrush(brush);

        // 绘制一个矩形
        painter.drawRect(50, 50, 100, 100);

        // 绘制一个文本
        painter.drawText(10, 180, "Hello, Qt!");
    }
    void enterEvent(QEvent*) override {
        // 当鼠标悬停在窗口上时，显示i图标和提示卡
        QToolTip::showText(mapToGlobal(QPoint(0, height())), "这是一个提示卡,QQQQQQQQQQQQQQQQQQ\nEEEEEEEEEE\nRRRRRRRRRRRRRRRR", this, QRect(), 5000);
    }

    void leaveEvent(QEvent*) override {
        // 当鼠标离开窗口时，隐藏提示卡
        QToolTip::hideText();
    }

    void mouseMoveEvent(QMouseEvent* event) override {
        // 在窗口上跟踪鼠标移动以更新提示卡位置
        QToolTip::showText(mapToGlobal(QPoint(0, height())), "这是一个提示卡,QQQQQQQQQQQQQQQQQQ\nEEEEEEEEEE", this, QRect(), 5000);
    }
};

#endif // TIPCARD_H
