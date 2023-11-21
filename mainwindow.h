#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ScrollableCardContainer.h"
#include <QTableWidget>
#include <QPushButton>
#include <QListView>
#include "ProcessManager.h"
#include "cpupara.h"
#include "labelwithtip.h"
#include "memorywidget.h"
#include "memorymanager.h"
#include "memoryusageprogressbar.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    void initKitBag();
    void initSomeUI();
    void setDefaultValue();
    void initProcessManager();
    void initMemoryManager();
    void initProcessFrame();
    void initAddArea();
    void initQueueArea();
    void initDetailArea();
    void initMemArea();
    void intMemDetailTable();
    void changeProcessArea(const int&num);
public slots:
    void addNewRow();
    void testChange();
private slots:
    void submitAdding();
signals:
    void addingSignal(int num);
private:
    Ui::MainWindow *ui;
    ProcessManager*processManager;
    MemoryManager*memoryManager;
    QString stylesheet="background-color: rgba(245, 245, 245, 0.8); border: 1px solid #ddd; border-radius: 5px; padding: 10px; QPushButton { background-color: transparent; }";
    std::string preparedfile="D:/OneDrive - csu.edu.cn/Codes/QtProjects/ProcessScheduling/preparedfile.csv";
    QPushButton*suspendButton;
    QPushButton* addButton;
    QPushButton* submitButton;
    QPushButton* csvButton;
    QPushButton*testButton;
    LabelWithTip*addAreaTipLabel;
    QFont*labelfont2;
    QFont*labelfont;
    QFont*tipfont;

    ScrollableCardContainer*backcon=nullptr;
    ScrollableCardContainer*readycon=nullptr;
    ScrollableCardContainer*suspendcon=nullptr;
    ScrollableCardContainer*swapcon=nullptr;
    QTableWidget* processingTable=nullptr;
    QTableWidget* processTable=nullptr;
    QTableWidget* memDetailTable=nullptr;
    QListView*detailView;
    MemoryWidget*memoryWidget;
    MemoryUsageProgressBar* membar;
    CPUPara*cpupara;
    int addingTableRows;
};
#endif // MAINWINDOW_H
