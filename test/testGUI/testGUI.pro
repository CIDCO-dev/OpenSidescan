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

CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT # So as not to display qDebug() outputs

CONFIG(debug, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT # So as not to display qDebug() outputs



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

    LIBS += "C:\\Program Files\\OpenCV-3.4.7\\opencv\\release\\install\\x64\\vc15\\lib\\opencv_flann347.lib"

    LIBS += "C:\\Program Files\\OpenCV-3.4.7\\opencv\\release\\install\\x64\\vc15\\lib\\opencv_highgui347.lib"
    LIBS += "C:\\Program Files\\OpenCV-3.4.7\\opencv\\release\\install\\x64\\vc15\\lib\\opencv_calib3d347.lib"
  }

SOURCES += \
    ../../src/thirdParty/MBES-lib/src/datagrams/DatagramParser.cpp \
    ../../src/thirdParty/MBES-lib/src/datagrams/DatagramParserFactory.cpp \
    ../../src/thirdParty/MBES-lib/src/datagrams/s7k/S7kParser.cpp \
    ../../src/thirdParty/MBES-lib/src/datagrams/kongsberg/KongsbergParser.cpp \
    ../../src/thirdParty/MBES-lib/src/datagrams/xtf/XtfParser.cpp \
    ../../src/thirdParty/MBES-lib/src/utils/NmeaUtils.cpp \
    ../../src/thirdParty/MBES-lib/src/utils/StringUtils.cpp \
    ../../src/thirdParty/MBES-lib/src/sidescan/SidescanPing.cpp \
    ../../src/thirdParty/opencv/apps/createsamples/utility.cpp \
    ../../src/OpenSidescan/machinelearning/dbscan.cpp \
    ../../src/OpenSidescan/project/project.cpp \
    ../../src/OpenSidescan/inventoryobject/inventoryobject.cpp \
    ../../src/OpenSidescan/sidescan/sidescanfile.cpp \
    ../../src/OpenSidescan/sidescan/sidescanimage.cpp \
    ../../src/OpenSidescan/sidescan/sidescanimager.cpp \
    ../../src/OpenSidescan/ui/docks/projectwindow/projecttreeitem.cpp \
    ../../src/OpenSidescan/ui/docks/projectwindow/projecttreemodel.cpp \
    ../../src/OpenSidescan/ui/docks/projectwindow/projectwindow.cpp \
    ../../src/OpenSidescan/ui/docks/channelpropertieswindow.cpp \
    ../../src/OpenSidescan/ui/docks/filepropertieswindow.cpp \
    ../../src/OpenSidescan/ui/docks/inventorywindow.cpp \
    ../../src/OpenSidescan/ui/about/aboutdialog.cpp \
    ../../src/OpenSidescan/detector/detector.cpp \
    ../../src/OpenSidescan/refactorme/progressdialognotclosingrightawayoncancel.cpp \
    ../../src/OpenSidescan/ui/detection/detectionwindow.cpp \
    ../../src/OpenSidescan/ui/detection/workerdetection.cpp \
    ../../src/OpenSidescan/ui/detection/monitorwindow.cpp \
    ../../src/OpenSidescan/ui/inventory/inventoryobjectmenu.cpp \
    ../../src/OpenSidescan/ui/inventory/inventoryobjectwindow.cpp \
    ../../src/OpenSidescan/ui/mainimage/imagetab.cpp \
    ../../src/OpenSidescan/ui/mainimage/imagetablabel.cpp \
    ../../src/OpenSidescan/ui/mainwindow/mainwindow.cpp \
    ../../src/OpenSidescan/ui/mainwindow/monitorthread.cpp \
    ../../src/OpenSidescan/ui/mainwindow/workerimportsidescanfiles.cpp \
    ../../src/OpenSidescan/ui/mainwindow/workeropenproject.cpp \
    ../../src/OpenSidescan/ui/training/trainingsampleswindow.cpp \
    ../../src/OpenSidescan/ui/training/workertrainingsamples.cpp \
    ../../src/OpenSidescan/utilities/opencvhelper.cpp \
    ../../src/OpenSidescan/utilities/qthelper.cpp \
    ../../src/OpenSidescan/ui/training/parameterscvCreateTrainingSamples.cpp \
    ../../src/OpenSidescan/ui/docks/vesselgeometrywindow.cpp \
    ../../src/OpenSidescan/detector/roidetector.cpp \
    testGUI.cpp

HEADERS  += ../../src/OpenSidescan/machinelearning/dbscan.h \
    ../../src/OpenSidescan/project/project.h \
    ../../src/OpenSidescan/inventoryobject/inventoryobject.h \
    ../../src/OpenSidescan/ui/training/parameterscvCreateTrainingSamples.h \
    ../../src/OpenSidescan/detector/detector.h \
    ../../src/OpenSidescan/ui/about/aboutdialog.h \
    ../../src/OpenSidescan/refactorme/boolwithmutex.h \
    ../../src/OpenSidescan/refactorme/progressdialognotclosingrightawayoncancel.h \
    ../../src/OpenSidescan/sidescan/sidescanfile.h \
    ../../src/OpenSidescan/sidescan/sidescanimage.h \
    ../../src/OpenSidescan/sidescan/sidescanimager.h \
    ../../src/OpenSidescan/ui/detection/detectionwindow.h \
    ../../src/OpenSidescan/ui/detection/workerdetection.h \
    ../../src/OpenSidescan/ui/detection/monitorwindow.h \
    ../../src/OpenSidescan/ui/docks/projectwindow/projecttreeitem.h \
    ../../src/OpenSidescan/ui/docks/projectwindow/projecttreemodel.h \
    ../../src/OpenSidescan/ui/docks/projectwindow/projectwindow.h \
    ../../src/OpenSidescan/ui/docks/channelpropertieswindow.h \
    ../../src/OpenSidescan/ui/docks/filepropertieswindow.h \
    ../../src/OpenSidescan/ui/docks/inventorywindow.h \
    ../../src/OpenSidescan/ui/inventory/inventoryobjectmenu.h \
    ../../src/OpenSidescan/ui/inventory/inventoryobjectwindow.h \
    ../../src/OpenSidescan/ui/mainimage/imagetab.h \
    ../../src/OpenSidescan/ui/mainimage/imagetablabel.h \
    ../../src/OpenSidescan/ui/mainwindow/mainwindow.h \
    ../../src/OpenSidescan/ui/mainwindow/monitorthread.h \
    ../../src/OpenSidescan/ui/mainwindow/workerimportsidescanfiles.h \
    ../../src/OpenSidescan/ui/mainwindow/workeropenproject.h \
    ../../src/OpenSidescan/ui/training/trainingsampleswindow.h \
    ../../src/OpenSidescan/ui/training/workertrainingsamples.h \
    ../../src/OpenSidescan/utilities/opencvhelper.h \
    ../../src/OpenSidescan/utilities/qthelper.h \
    ../../src/OpenSidescan/ui/docks/vesselgeometrywindow.h \
    ../../src/OpenSidescan/detector/roidetector.h

FORMS    += 	../../src/OpenSidescan/ui/mainwindow/mainwindow.ui \
                ../../src/OpenSidescan/ui/about/aboutdialog.ui

RESOURCES +=  ../../src/OpenSidescan/resources.qrc

RESOURCES +=  ../../src/thirdParty/QDarkStyleSheet/qdarkstyle/style.qrc

INCLUDEPATH += ../../src/OpenSidescan/

CONFIG+=link_pkgconfig

unix{
        PKGCONFIG+=opencv eigen3
}
