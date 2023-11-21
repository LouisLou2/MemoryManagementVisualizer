#ifndef PROCESSCARD_H
#define PROCESSCARD_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>

class ProcessCard : public QWidget {
    Q_OBJECT
public:
    ProcessCard(const QString& name, int totalTime, const QString& creationTime, int priority, const QString& status, QWidget* parent = nullptr);
    ProcessCard();
    void paintEvent(QPaintEvent*)override;
private:
    QLabel* nameLabel;
    QLabel* totalTimeLabel;
    QLabel* creationTimeLabel;
    QLabel* priorityLabel;
    QLabel* statusLabel;
};

#endif // PROCESSCARD_H
