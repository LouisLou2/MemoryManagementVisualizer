#include "labelwithtip.h"
#include <QtWidgets>
#include <QCursor>
LabelWithTip::LabelWithTip(const QString& text,QWidget* parent)
        : QLabel(text, parent)
    {
    tiptext=text;
    }
void LabelWithTip::enterEvent(QEvent* event)
{
    QLabel::enterEvent(event);
    QLabel::enterEvent(event);

    // 当鼠标悬停时，显示提示
    QToolTip::showText(mapToGlobal(QPoint(0, height())),tiptext);
}

void LabelWithTip::leaveEvent(QEvent* event)
{
    QLabel::leaveEvent(event);

    // 当鼠标离开时，隐藏提示
    QToolTip::hideText();
}
