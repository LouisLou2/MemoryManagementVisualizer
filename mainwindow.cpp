#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QString>
#include <QTableWidget>
#include <QTimer>
#include <QHeaderView>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QStringList>
#include <QComboBox>
#include <QLineEdit>
#include "myitemdelegate.h"
#include "processmanager.h"
#include "ScrollableCardContainer.h"
#include "myitem.h"
#include "tipcard.h"
#include "memorywidget.h"
#include "utils/parseutil.h"
//构造函数调用子函数顺序不能乱
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("CPU Scheduling");
    initKitBag();
    initSomeUI();
    setDefaultValue();
    initQueueArea();
    initProcessFrame();
    initAddArea();
    initDetailArea();
    initMemArea();
    initMemoryManager();
    initProcessManager();
    //ui->allMemSizeEdit->setEnabled(false);//----------------------------------目前没想弄这个
    connect(ui->startButton,&QPushButton::clicked,this,[&]() {
        processManager->begin();
        ui->revertButton->setEnabled(true);
        ui->startButton->setEnabled(false);
        ui->pauseButton->setEnabled(true);
        ui->continueButton->setEnabled(false);
        ui->CorespinBox->setEnabled(false);
        ui->ThreadsspinBox->setEnabled(false);
        ui->fitAlgoCombo->setEnabled(false);
        ui->allMemSizeBtn->setEnabled(false);
    });
    connect(ui->pauseButton,&QPushButton::clicked,this,[&]() {
        processManager->pause();
        ui->pauseButton->setEnabled(false);
        ui->continueButton->setEnabled(true);
    });
    connect(ui->continueButton,&QPushButton::clicked,this,[&]() {
        processManager->continueProcess();
        ui->continueButton->setEnabled(false);
        ui->pauseButton->setEnabled(true);
    });
    connect(ui->revertButton,&QPushButton::clicked,this,[&]() {
        ui->startButton->setEnabled(true);
        ui->revertButton->setEnabled(false);
        ui->pauseButton->setEnabled(false);
        ui->continueButton->setEnabled(false);
        ui->CorespinBox->setEnabled(true);
        ui->ThreadsspinBox->setEnabled(true);
        processManager->revertToOrigin();
        ui->fitAlgoCombo->setEnabled(true);
        ui->allMemSizeBtn->setEnabled(true);
    });
    connect(ui->suspendButton,&QPushButton::clicked,processManager,&ProcessManager::addSuspendQueue);
    connect(ui->releaseSusButton,&QPushButton::clicked,processManager,[&]() {
        processManager->releaseFromSuspendQueue();
    });
    connect(ui->releaseAllSusButton,&QPushButton::clicked,processManager,[&]() {
        processManager->releaseFromSuspendQueue(-1);
    });
    connect(ui->intervalspinBox, QOverload<int>::of(&QSpinBox::valueChanged), this,[&](int value) {
        if(value>0) processManager->setServeTime(value*1000);
    });
    connect(ui->CorespinBox,QOverload<int>::of(&QSpinBox::valueChanged), this,[&](int value) {
        if(value<1)return;
        cpupara->setCoreCount(value);
        ui->ThreadsspinBox->setMinimum(value);
        changeProcessArea(value);
    });
    connect(ui->ThreadsspinBox,QOverload<int>::of(&QSpinBox::valueChanged), this,[&](int value) {
        if(value<ui->CorespinBox->value())return;
        cpupara->setcpuParallelThreads(value);
    });
    connect(ui->fitAlgoCombo,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),this->memoryManager,&MemoryManager::changeAllocateAlgo);
//    connect(ui->allMemSizeEdit,&QLineEdit::textChanged,this,[&](QString value) {
//        bool ok=false;
//        int intvalue=value.toInt(&ok);
//        if(!ok||intvalue<8*1024*1024)return;
//        OSobject::ChangeAllMemSize(intvalue);
//        memoryManager->changeAllMemSize();
//    });
    connect(ui->allMemSizeBtn,&QPushButton::clicked,this,[&]() {
        QString value=ui->allMemSizeEdit->text();
        bool ok=false;
        int intvalue=value.toInt(&ok);
        if(!ok||(intvalue<8*1024*1024))return;
        OSobject::ChangeAllMemSize(intvalue);
        memoryManager->changeAllMemSize();
    });
    //这个如果放在addRowArea，此时ProcessManager还未初始化，并且位置确实不能动，所以在这里connect
    connect(csvButton,&QPushButton::clicked,processManager,&ProcessManager::readRowsFromFile);
    connect(ui->suspendProcessingButton,&QPushButton::clicked,processManager,&ProcessManager::addSuspendQueueFromProcessing);
}
void MainWindow::initKitBag(){
    labelfont=new QFont();
    labelfont->setBold(false);
    labelfont->setFamily("Microsoft YaHei");
    labelfont->setPointSize(12);
    labelfont2=new QFont(*labelfont);
    labelfont2->setPointSize(9);
    tipfont=new QFont();
    tipfont->setFamily("Microsoft YaHei");
    tipfont->setPointSize(10);
}
void MainWindow::initSomeUI(){
    ui->nowProcessingLabel->setText("正在执行");
    ui->nowProcessingLabel->setFont(*labelfont);
    ui->detailLabel->setText("详细信息");
    ui->detailLabel->setFont(*labelfont);
    ui->suspendButton->setFont(*labelfont2);
    ui->releaseSusButton->setFont(*labelfont2);
    ui->releaseAllSusButton->setFont(*labelfont2);
    ui->suspendProcessingButton->setFont(*labelfont2);
    ui->startButton->setFont(*labelfont2);
    ui->revertButton->setFont(*labelfont2);
    ui->pauseButton->setFont(*labelfont2);
    ui->continueButton->setFont(*labelfont2);
    ui->intervalLabel->setFont(*labelfont2);
    ui->CoreLabel->setFont(*labelfont2);
    ui->threadsLabel->setFont(*labelfont2);
    ui->allMemSize->setText(getSizeStr(OSobject::allMemorySize));
}
void MainWindow::setDefaultValue(){
    cpupara=new CPUPara(4,8);//指定cpu核心数是4
    ui->revertButton->setEnabled(false);
    ui->pauseButton->setEnabled(false);
    ui->continueButton->setEnabled(false);
    ui->intervalspinBox->setValue(3);
    ui->intervalspinBox->setMinimum(1);
    ui->CorespinBox->setMinimum(1);
    ui->ThreadsspinBox->setMinimum(ui->CorespinBox->minimum());
    ui->CorespinBox->setValue(cpupara->getCoreCount());
    ui->ThreadsspinBox->setValue(cpupara->getcpuParallelThreads());
    ui->fitAlgoCombo->setCurrentIndex(0);
    QIntValidator *validator = new QIntValidator(this);
    validator->setRange(8*1024*1024, 1000*1024*1024); // 设置整数范围
    // 将 validator 应用到你的 QLineEdit
    ui->allMemSizeEdit->setValidator(validator);
}
void MainWindow::initQueueArea(){
    QLabel* headline1 = new QLabel("  进程名    优先级    剩余时间", this);
    QLabel* headline2 = new QLabel(headline1->text(),this);
    QLabel* headline3 = new QLabel(headline1->text(), this);
    QLabel* headline4 = new QLabel(headline1->text(), this);
    headline1->setFont(*labelfont2);
    headline2->setFont(*labelfont2);
    headline3->setFont(*labelfont2);
    headline4->setFont(*labelfont2);
    //后备队列区
    QVBoxLayout *blayout = new QVBoxLayout(this);
    ui->back->setLayout(blayout);
    QLabel* backLabel = new QLabel("后备队列", this);
     backLabel->setFont(*labelfont);
     backcon=new ScrollableCardContainer(this);
     blayout->addWidget(backLabel);
     blayout->addWidget(headline1);
     blayout->addWidget(backcon);
    //就绪队列区
     QVBoxLayout *rlayout = new QVBoxLayout(this);
     ui->ready->setLayout(rlayout);
     QLabel* readyLabel = new QLabel("就绪队列", this);
      readyLabel->setFont(*labelfont);
      readycon=new ScrollableCardContainer(this);
      rlayout->addWidget(readyLabel);
      rlayout->addWidget(headline2);
      rlayout->addWidget(readycon);
    //挂起队列
      QVBoxLayout *slayout = new QVBoxLayout(this);
      ui->suspend->setLayout(slayout);
      QLabel* suspendLabel = new QLabel("挂起队列", this);
      suspendLabel->setFont(*labelfont);
      suspendcon=new ScrollableCardContainer(this);
      slayout->addWidget(suspendLabel);
      slayout->addWidget(headline3);
      slayout->addWidget(suspendcon);
      //挂起队列
      QVBoxLayout *swaplayout = new QVBoxLayout(this);
      ui->swaping->setLayout(swaplayout);
      QLabel* swapLabel = new QLabel("交换区", this);
      swapLabel->setFont(*labelfont);
      swapcon=new ScrollableCardContainer(this);
      swaplayout->addWidget(swapLabel);
      swaplayout->addWidget(headline4);
      swaplayout->addWidget(swapcon);
}
void MainWindow::initDetailArea(){
      detailView=new QListView(this);
      // 创建一个QStandardItemModel用于填充数据
      QStandardItemModel*model=new QStandardItemModel(this);
      // 设置QListView的模型为QStandardItemModel
      detailView->setModel(model);
      // 使用样式表来设置圆角边框、边框颜色和粗细
      detailView->setStyleSheet("QListView {"
                             "border: 2px solid black; /* 边框颜色和粗细 */"
                             "border-radius: 20px; /* 圆角半径 */"
                             "}");
      // 创建自定义委托
      MyItemDelegate*itemDelegat=new MyItemDelegate(this);
      detailView->setItemDelegate(itemDelegat);
      ui->detailLayout->addWidget(detailView);
}
void MainWindow::initMemArea(){
      memoryWidget=new MemoryWidget();
      ui->memLayout->addWidget(memoryWidget);
      membar=new MemoryUsageProgressBar();
      ui->memProgressLayout->addWidget(membar);
      intMemDetailTable();
}
void MainWindow::intMemDetailTable(){
      // 创建一个QTableWidget
      memDetailTable= new QTableWidget(4, 3); // 4行4列
      // 设置QTableWidget为只读
      memDetailTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

      // 添加表头
      QStringList headerLabels = {"进程名", "起址", "大小" };
      memDetailTable->setHorizontalHeaderLabels(headerLabels);
      for(int i=0;i<cpupara->getCoreCount();++i){
          // 添加内容
          QTableWidgetItem* core1Item = new QTableWidgetItem("");
          memDetailTable->setItem(i, 0, core1Item);
          for(int j=1;j<4;++j){
              QTableWidgetItem* item = new QTableWidgetItem("");
              memDetailTable->setItem(i, j, item);
          }
      }
      ui->memDetailLayout->addWidget(memDetailTable);

      // 设置表格的列宽适应内容
      memDetailTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}
void MainWindow::initMemoryManager(){
    memoryManager=new MemoryManager(ui->allMemSize,ui->memUsingSize,ui->memLeftSize,membar,memoryWidget,memDetailTable,this);
}
void MainWindow::initProcessManager(){
  if(!backcon||!readycon||!suspendcon||!processingTable||!processTable||!detailView){
      std::cout<<"processManager接管的容器指针空！！"<<std::endl;
      exit(1);
  }
  processManager=new ProcessManager(cpupara,preparedfile,backcon,readycon,suspendcon,swapcon,processingTable,processTable,detailView,this);
  processManager->takeMemoryManager(memoryManager);
}

void MainWindow::initProcessFrame() {
    // 创建一个QTableWidget
    processingTable= new QTableWidget(4,cpupara->getCoreCount(), ui->processingFrame); // 4行4列
    // 设置QTableWidget为只读
    processingTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // 添加表头
    QStringList headerLabels = { "CPU核心", "进程名", "优先级", "待处理次数" };
    processingTable->setHorizontalHeaderLabels(headerLabels);
    for(int i=0;i<cpupara->getCoreCount();++i){
      // 添加内容
      QTableWidgetItem* core1Item = new QTableWidgetItem(QString("Core").append(QString::number(i+1)));
      processingTable->setItem(i, 0, core1Item);
      for(int j=1;j<4;++j){
          QTableWidgetItem* item = new QTableWidgetItem("");
          processingTable->setItem(i, j, item);
      }
    }
    // 将表格放入QFrame
    QVBoxLayout* frameLayout = new QVBoxLayout(ui->processingFrame);
    frameLayout->addWidget(processingTable);

    // 设置表格的列宽适应内容
    processingTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}
void MainWindow::changeProcessArea(const int&num){
    if(num<1)return;
    int nowrow=processingTable->rowCount();
    if(nowrow==num)return;
    if(num>nowrow){
      while(num>nowrow){
          processingTable->insertRow(nowrow);
          QTableWidgetItem* core1Item = new QTableWidgetItem(QString("Core").append(QString::number(nowrow+1)));
          processingTable->setItem(nowrow, 0, core1Item);
          for(int j=1;j<4;++j){
              QTableWidgetItem* item = new QTableWidgetItem("");
              processingTable->setItem(nowrow, j, item);
          }
          ++nowrow;
      }
      return;
    }
    while(num<nowrow){
      processingTable->removeRow(nowrow-1);
      --nowrow;
    }
    processingTable->update();
}
void MainWindow::initAddArea() {
    processTable = new QTableWidget(this);
    processTable->setRowCount(0); // 初始化表格的行数
    processTable->setColumnCount(4); // 设置表格的列数

    // 设置表头标签
    QStringList headers;
    headers << "进程名" << "占用时间" << "优先级"<<"大小";
    processTable->setHorizontalHeaderLabels(headers);
    processTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    addAreaTipLabel=new LabelWithTip("重复的进程名，运行时间小于1的进程，以及进程大小大于，除操作系统外内存空间的进程\n会被过滤而不会被创建",this);
    QPixmap oripixmap(":/new/prefix1/tips.svg");
    QPixmap pixmap = oripixmap.scaled(50, 50, Qt::KeepAspectRatio);
    // 设置 QLabel 的内容为 QPixmap
    addAreaTipLabel->setPixmap(pixmap);

    addButton = new QPushButton("添加进程",this); // 创建一个按钮
    submitButton=new QPushButton("提交",this);
    csvButton =new QPushButton("由csv文件",this);
    QHBoxLayout* buttonLayout = new QHBoxLayout(this);

    buttonLayout->addWidget(addAreaTipLabel);
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(submitButton);
    buttonLayout->addWidget(csvButton);
    // 创建一个连接，点击按钮时执行添加新行的槽函数
    connect(addButton, &QPushButton::clicked, this, &MainWindow::addNewRow);
    connect(submitButton, &QPushButton::clicked, this, &MainWindow::submitAdding);
    // 将表格和按钮添加到布局
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(processTable);
    layout->addLayout(buttonLayout);
    ui->addframe->setLayout(layout);
}
MainWindow::~MainWindow()
{
    delete ui;
    if(labelfont){
      delete labelfont;
      labelfont=nullptr;
    }
    if(labelfont2){
      delete labelfont2;
      labelfont2=nullptr;
    }
    if(tipfont){
      delete tipfont;
      tipfont =nullptr;
    }
    if(cpupara){
      delete cpupara;
      cpupara=nullptr;
    }

}
void MainWindow::addNewRow(){
    int row = processTable->rowCount(); // 获取当前行数
    processTable->insertRow(row);
    processTable->setItem(row, 0, new QTableWidgetItem(""));
    processTable->setItem(row, 1, new QTableWidgetItem("0"));
    processTable->setItem(row, 2, new QTableWidgetItem("0"));
    processTable->setItem(row, 3, new QTableWidgetItem("0"));
    addingTableRows=row+1;
}
void MainWindow::submitAdding(){
    if(addingTableRows>0)
    processManager->addRows(addingTableRows);
}
void MainWindow::testChange(){
    processManager->backcon->pop();
    processManager->backcon->getTop()->setPriority(100000);
}
