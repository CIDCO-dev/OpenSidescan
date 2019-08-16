#-------------------------------------------------
#
# Project created by QtCreator 2019-08-15T12:34:57
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OpenSidescan
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    sidescanimager.cpp \
    ../thirdParty/MBES-lib/src/datagrams/DatagramParser.cpp \
    ../thirdParty/MBES-lib/src/datagrams/DatagramParserFactory.cpp \
    ../thirdParty/MBES-lib/src/datagrams/s7k/S7kParser.cpp \
    ../thirdParty/MBES-lib/src/datagrams/kongsberg/KongsbergParser.cpp \
    ../thirdParty/MBES-lib/src/datagrams/xtf/XtfParser.cpp \
    ../thirdParty/MBES-lib/src/svp/SoundVelocityProfile.cpp \
    ../thirdParty/MBES-lib/src/utils/NmeaUtils.cpp \
    ../thirdParty/MBES-lib/src/utils/StringUtils.cpp \
    opencvhelper.cpp

HEADERS  += mainwindow.h \
    sidescanimager.h \
    opencvhelper.h

FORMS    += mainwindow.ui

RESOURCES += \
    resources.qrc

CONFIG+=link_pkgconfig
PKGCONFIG+=opencv eigen3
