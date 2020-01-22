#-------------------------------------------------
#
# Project created by QtCreator 2019-08-15T12:34:57
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QT += testlib

TARGET = testGUI
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

#CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT # So as not to display qDebug() outputs

#CONFIG(debug, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT # So as not to display qDebug() outputs



# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

win32 {
    RC_ICONS = ..\\..\\src\\OpenSidescan\\resources\\cidco_icon.ico

    INCLUDEPATH += "C:\\Program Files (x86)\\Eigen3\\include\\eigen3" \
                    "C:\\Program Files\\OpenCV-3.4.7\\opencv\\release\\install\\include"

    LIBS += "C:\\Program Files\\OpenCV-3.4.7\\opencv\\release\\install\\x64\\vc15\\lib\\opencv_core347.lib"
    LIBS += "C:\\Program Files\\OpenCV-3.4.7\\opencv\\release\\install\\x64\\vc15\\lib\\opencv_features2d347.lib"
    LIBS += "C:\\Program Files\\OpenCV-3.4.7\\opencv\\release\\install\\x64\\vc15\\lib\\opencv_imgcodecs347.lib"
    LIBS += "C:\\Program Files\\OpenCV-3.4.7\\opencv\\release\\install\\x64\\vc15\\lib\\opencv_imgproc347.lib"
    LIBS += "C:\\Program Files\\OpenCV-3.4.7\\opencv\\release\\install\\x64\\vc15\\lib\\opencv_ml347.lib"
    LIBS += "C:\\Program Files\\OpenCV-3.4.7\\opencv\\release\\install\\x64\\vc15\\lib\\opencv_photo347.lib"
  }

SOURCES += testGUI.cpp \
        ../../src/OpenSidescan/mainwindow.cpp \
    ../../src/OpenSidescan/sidescanimager.cpp \
    ../../src/thirdParty/MBES-lib/src/datagrams/DatagramParser.cpp \
    ../../src/thirdParty/MBES-lib/src/datagrams/DatagramParserFactory.cpp \
    ../../src/thirdParty/MBES-lib/src/datagrams/s7k/S7kParser.cpp \
    ../../src/thirdParty/MBES-lib/src/datagrams/kongsberg/KongsbergParser.cpp \
    ../../src/thirdParty/MBES-lib/src/datagrams/xtf/XtfParser.cpp \
    ../../src/thirdParty/MBES-lib/src/utils/NmeaUtils.cpp \
    ../../src/thirdParty/MBES-lib/src/utils/StringUtils.cpp \
    ../../src/thirdParty/MBES-lib/src/sidescan/SidescanPing.cpp \
    ../../src/OpenSidescan/opencvhelper.cpp \
    ../../src/OpenSidescan/imagetab.cpp \
    ../../src/OpenSidescan/aboutdialog.cpp \
    ../../src/OpenSidescan/dbscan.cpp \
    ../../src/OpenSidescan/sidescanimage.cpp \
    ../../src/OpenSidescan/projectwindow.cpp \
    ../../src/OpenSidescan/sidescanfile.cpp \
    ../../src/OpenSidescan/project.cpp \
    ../../src/OpenSidescan/georeferencedobject.cpp \
    ../../src/OpenSidescan/filepropertieswindow.cpp \
    ../../src/OpenSidescan/detectionwindow.cpp \
    ../../src/OpenSidescan/inventorywindow.cpp \
    ../../src/OpenSidescan/qthelper.cpp \
    ../../src/OpenSidescan/imagetablabel.cpp \
    ../../src/OpenSidescan/georeferencedobjectwindow.cpp \
    ../../src/OpenSidescan/georeferencedobjectmenu.cpp \
    ../../src/OpenSidescan/workerimportsidescanfiles.cpp \
    ../../src/OpenSidescan/workeropenproject.cpp \
    ../../src/OpenSidescan/workerdetection.cpp \
    ../../src/OpenSidescan/projecttreeitem.cpp \
    ../../src/OpenSidescan/projecttreemodel.cpp

HEADERS  += ../../src/OpenSidescan/mainwindow.h \
    ../../src/OpenSidescan/sidescanimager.h \
    ../../src/OpenSidescan/opencvhelper.h \
    ../../src/OpenSidescan/imagetab.h \
    ../../src/OpenSidescan/aboutdialog.h \
    ../../src/OpenSidescan/dbscan.h \
    ../../src/OpenSidescan/sidescanimage.h \
    ../../src/OpenSidescan/projectwindow.h \
    ../../src/OpenSidescan/sidescanfile.h \
    ../../src/OpenSidescan/project.h \
    ../../src/OpenSidescan/georeferencedobject.h \
    ../../src/OpenSidescan/filepropertieswindow.h \
    ../../src/OpenSidescan/detectionwindow.h \
    ../../src/OpenSidescan/inventorywindow.h \
    ../../src/OpenSidescan/qthelper.h \
    ../../src/OpenSidescan/imagetablabel.h \
    ../../src/OpenSidescan/georeferencedobjectwindow.h \
    ../../src/OpenSidescan/georeferencedobjectmenu.h \
    ../../src/OpenSidescan/workerimportsidescanfiles.h \
    ../../src/OpenSidescan/workeropenproject.h \
    ../../src/OpenSidescan/workerdetection.h \
    ../../src/OpenSidescan/projecttreeitem.h \
    ../../src/OpenSidescan/projecttreemodel.h

FORMS    += 	../../src/OpenSidescan/mainwindow.ui \
    		../../src/OpenSidescan/aboutdialog.ui

RESOURCES +=  ../../src/OpenSidescan/resources.qrc

RESOURCES +=  ../../src/thirdParty/QDarkStyleSheet/qdarkstyle/style.qrc

CONFIG+=link_pkgconfig

unix{
        PKGCONFIG+=opencv eigen3
}
