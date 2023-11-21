#include "maintabwindow.h"
#include "ui_maintabwindow.h"
#include "mainwindow.h"
#include <QTabWidget>
MainTabWindow::MainTabWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainTabWindow)
{
    ui->setupUi(this);
    QTabWidget* tabw=new QTabWidget;
    MainWindow*pri=new MainWindow(this);
    tabw->addTab(pri,"优先级调度");
    tabw->setMinimumSize(2500,1400);
    ui->alayout->addWidget(tabw);
    adjustSize();
}

MainTabWindow::~MainTabWindow()
{
    delete ui;
}
