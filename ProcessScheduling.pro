QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    OSobject.cpp \
    ScrollableCardContainer.cpp \
    cardwidget.cpp \
    derivedpriorityqueue.cpp \
    labelwithtip.cpp \
    main.cpp \
    maintabwindow.cpp \
    mainwindow.cpp \
    memoryblock.cpp \
    memoryguimanager.cpp \
    memorymanager.cpp \
    memoryusageprogressbar.cpp \
    memorywidget.cpp \
    myitem.cpp \
    myitemdelegate.cpp \
    proc.cpp \
    processcard.cpp \
    processmanager.cpp \
    tipcard.cpp \
    utils/mytimer.cpp \
    utils/parseutil.cpp \
    utils\timeutil.cpp

HEADERS += \
    OSobject.h \
    ScrollableCardContainer.h \
    cardwidget.h \
    cpupara.h \
    derivedpriorityqueue.h \
    labelwithtip.h \
    maintabwindow.h \
    mainwindow.h \
    memoryblock.h \
    memoryguimanager.h \
    memorymanager.h \
    memoryusageprogressbar.h \
    memorywidget.h \
    myitem.h \
    myitemdelegate.h \
    proc.h \
    processcard.h \
    processmanager.h \
    tipcard.h \
    utils/mytimer.h \
    utils/parseutil.h \
    utils\timeutil.h

FORMS += \
    maintabwindow.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    otherproc.csv \
    preparedfile.csv \
    tips.svg

RESOURCES += \
    icons.qrc
