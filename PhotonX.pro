QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    appstate.cpp \
    carousel.cpp \
    db.cpp \
    main.cpp \
    mainimg.cpp \
    mainwindow.cpp \
    previewimg.cpp

HEADERS += \
    addfilterdialog.h \
    filter.h \
    image.h \
    star.h \
    appstate.h \
    carousel.h \
    db.h \
    mainimg.h \
    mainwindow.h \
    previewimg.h \
    tag.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

RESOURCES += \
    resources.qrc
