#-------------------------------------------------
#
# Project created by QtCreator 2019-08-15T12:34:57
#
#-------------------------------------------------

QT       += core gui xml

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

win32 {
    RC_ICONS = resources\\cidco_icon.ico

    INCLUDEPATH += "C:\\Program Files (x86)\\Eigen3\\include\\eigen3" \
                    "C:\\Program Files\\OpenCV-3.4.7\\opencv\\release\\install\\include"

    LIBS += "C:\\Program Files\\OpenCV-3.4.7\\opencv\\release\\install\\x64\\vc15\\lib\\opencv_core347.lib"
    LIBS += "C:\\Program Files\\OpenCV-3.4.7\\opencv\\release\\install\\x64\\vc15\\lib\\opencv_features2d347.lib"
    LIBS += "C:\\Program Files\\OpenCV-3.4.7\\opencv\\release\\install\\x64\\vc15\\lib\\opencv_imgcodecs347.lib"
    LIBS += "C:\\Program Files\\OpenCV-3.4.7\\opencv\\release\\install\\x64\\vc15\\lib\\opencv_imgproc347.lib"
    LIBS += "C:\\Program Files\\OpenCV-3.4.7\\opencv\\release\\install\\x64\\vc15\\lib\\opencv_ml347.lib"
    LIBS += "C:\\Program Files\\OpenCV-3.4.7\\opencv\\release\\install\\x64\\vc15\\lib\\opencv_photo347.lib"
  }

SOURCES += main.cpp\
        mainwindow.cpp \
    sidescanimager.cpp \
    ../thirdParty/MBES-lib/src/datagrams/DatagramParser.cpp \
    ../thirdParty/MBES-lib/src/datagrams/DatagramParserFactory.cpp \
    ../thirdParty/MBES-lib/src/datagrams/s7k/S7kParser.cpp \
    ../thirdParty/MBES-lib/src/datagrams/kongsberg/KongsbergParser.cpp \
    ../thirdParty/MBES-lib/src/datagrams/xtf/XtfParser.cpp \
    ../thirdParty/MBES-lib/src/utils/NmeaUtils.cpp \
    ../thirdParty/MBES-lib/src/utils/StringUtils.cpp \
    ../thirdParty/MBES-lib/src/sidescan/SidescanPing.cpp \
    opencvhelper.cpp \
    imagetab.cpp \
    aboutdialog.cpp \
    dbscan.cpp \
    sidescanimage.cpp \
    projectwindow.cpp \
    sidescanfile.cpp \
    project.cpp \
    georeferencedobject.cpp \
    filepropertieswindow.cpp \
    detectionwindow.cpp \
    inventorywindow.cpp \
    qthelper.cpp \
    imagetablabel.cpp \
    georeferencedobjectwindow.cpp \
    georeferencedobjectmenu.cpp

HEADERS  += mainwindow.h \
    sidescanimager.h \
    opencvhelper.h \
    imagetab.h \
    aboutdialog.h \
    dbscan.h \
    sidescanimage.h \
    projectwindow.h \
    sidescanfile.h \
    project.h \
    georeferencedobject.h \
    filepropertieswindow.h \
    detectionwindow.h \
    inventorywindow.h \
    qthelper.h \
    imagetablabel.h \
    georeferencedobjectwindow.h \
    georeferencedobjectmenu.h

FORMS    += mainwindow.ui \
    aboutdialog.ui

RESOURCES +=  resources.qrc

RESOURCES +=  ../thirdParty/QDarkStyleSheet/qdarkstyle/style.qrc

CONFIG+=link_pkgconfig

unix{
        PKGCONFIG+=opencv eigen3
}
