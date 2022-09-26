QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    sshproc.cpp \
    testdialog.cpp \
    threadStart.cpp \
    uihelper.cpp

HEADERS += \
    incl.h \
    mainwindow.h \
    sshproc.h \
    testdialog.h \
    threadStart.h \
    uihelper.h

FORMS += \
    mainwindow.ui \
    testdialog.ui

QMAKE_CXXFLAGS += /utf-8

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target



win32: LIBS += -L$$PWD/lib/ -llibssh2

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.
