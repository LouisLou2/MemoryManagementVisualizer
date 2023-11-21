#ifndef LABELWITHTIP_H
#define LABELWITHTIP_H

#include <QtWidgets>
class LabelWithTip : public QLabel
{
public:
   LabelWithTip(const QString& text,QWidget* parent = nullptr);
protected:
    void enterEvent(QEvent* event) override;

    void leaveEvent(QEvent* event) override;
private:
    QString tiptext;
};
#endif // LABELWITHTIP_H
