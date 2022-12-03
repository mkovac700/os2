QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    aboutdialog.cpp \
    encdecdialog.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    aboutdialog.h \
    encdecdialog.h \
    mainwindow.h

FORMS += \
    aboutdialog.ui \
    encdecdialog.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    icons.qrc \
    images.qrc

linux-g++* {
    INCLUDEPATH += /home/WebDiP_05/chilkat-9.5.0-x86_64-linux-gcc/include
    LIBS += -L/home/WebDiP_05/chilkat-9.5.0-x86_64-linux-gcc/lib -lchilkat-9.5.0 -lresolv -lpthread
}

win32 {
    win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../Libraries/chilkat-mingw-x86_64-10.0.0-posix-seh/ -llibchilkat-9.5.0 -lwsock32 -lcrypt32 -lws2_32 -ldnsapi
    else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../Libraries/chilkat-mingw-x86_64-10.0.0-posix-seh/ -llibchilkat-9.5.0 -lwsock32 -lcrypt32 -lws2_32 -ldnsapi

    INCLUDEPATH += $$PWD/../../Libraries/chilkat-mingw-x86_64-10.0.0-posix-seh/include
    DEPENDPATH += $$PWD/../../Libraries/chilkat-mingw-x86_64-10.0.0-posix-seh/include

    RC_ICONS = appicon.ico
}
