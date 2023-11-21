#include "mainwindow.h"
#include "maintabwindow.h"
#include <QApplication>
#include <QtWidgets>
#include <ScrollableCardContainer.h>
#include <QString>
int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
//    MainTabWindow w;
//    w.show();
    MainWindow w;
    w.show();
    return a.exec();
}
