/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionImport;
    QAction *actionAbout;
    QAction *actionQuit;
    QAction *actionFindObjects;
    QAction *actionShowProjectWindow;
    QAction *actionShowFilePropertiesWindow;
    QAction *actionShowObjectInventoryWindow;
    QAction *actionNewProject;
    QAction *actionOpenProject;
    QAction *actionSaveProject;
    QAction *actionSaveAs;
    QAction *actionExportKmlFile;
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuHelp;
    QMenu *menuObject_Inventory;
    QMenu *menuExport_Inventory;
    QMenu *menuWindows;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(500, 382);
        QIcon icon;
        icon.addFile(QStringLiteral(":/Images/resources/cidco_icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        actionImport = new QAction(MainWindow);
        actionImport->setObjectName(QStringLiteral("actionImport"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/Images/resources/import.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionImport->setIcon(icon1);
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName(QStringLiteral("actionAbout"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/Images/resources/help.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionAbout->setIcon(icon2);
        actionQuit = new QAction(MainWindow);
        actionQuit->setObjectName(QStringLiteral("actionQuit"));
        actionFindObjects = new QAction(MainWindow);
        actionFindObjects->setObjectName(QStringLiteral("actionFindObjects"));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/Images/resources/marker.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionFindObjects->setIcon(icon3);
        actionShowProjectWindow = new QAction(MainWindow);
        actionShowProjectWindow->setObjectName(QStringLiteral("actionShowProjectWindow"));
        actionShowFilePropertiesWindow = new QAction(MainWindow);
        actionShowFilePropertiesWindow->setObjectName(QStringLiteral("actionShowFilePropertiesWindow"));
        actionShowObjectInventoryWindow = new QAction(MainWindow);
        actionShowObjectInventoryWindow->setObjectName(QStringLiteral("actionShowObjectInventoryWindow"));
        actionNewProject = new QAction(MainWindow);
        actionNewProject->setObjectName(QStringLiteral("actionNewProject"));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/Images/resources/new.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionNewProject->setIcon(icon4);
        actionOpenProject = new QAction(MainWindow);
        actionOpenProject->setObjectName(QStringLiteral("actionOpenProject"));
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/Images/resources/open.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionOpenProject->setIcon(icon5);
        actionSaveProject = new QAction(MainWindow);
        actionSaveProject->setObjectName(QStringLiteral("actionSaveProject"));
        QIcon icon6;
        icon6.addFile(QStringLiteral(":/Images/resources/save.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSaveProject->setIcon(icon6);
        actionSaveAs = new QAction(MainWindow);
        actionSaveAs->setObjectName(QStringLiteral("actionSaveAs"));
        QIcon icon7;
        icon7.addFile(QStringLiteral(":/Images/resources/save-as.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSaveAs->setIcon(icon7);
        actionExportKmlFile = new QAction(MainWindow);
        actionExportKmlFile->setObjectName(QStringLiteral("actionExportKmlFile"));
        QIcon icon8;
        icon8.addFile(QStringLiteral(":/Images/resources/kml.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionExportKmlFile->setIcon(icon8);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 500, 25));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QStringLiteral("menuHelp"));
        menuObject_Inventory = new QMenu(menuBar);
        menuObject_Inventory->setObjectName(QStringLiteral("menuObject_Inventory"));
        menuExport_Inventory = new QMenu(menuObject_Inventory);
        menuExport_Inventory->setObjectName(QStringLiteral("menuExport_Inventory"));
        menuWindows = new QMenu(menuBar);
        menuWindows->setObjectName(QStringLiteral("menuWindows"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        mainToolBar->setMovable(false);
        mainToolBar->setFloatable(false);
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuObject_Inventory->menuAction());
        menuBar->addAction(menuWindows->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionNewProject);
        menuFile->addAction(actionOpenProject);
        menuFile->addAction(actionSaveProject);
        menuFile->addAction(actionSaveAs);
        menuFile->addSeparator();
        menuFile->addAction(actionImport);
        menuFile->addSeparator();
        menuFile->addAction(actionQuit);
        menuHelp->addAction(actionAbout);
        menuObject_Inventory->addAction(actionFindObjects);
        menuObject_Inventory->addAction(menuExport_Inventory->menuAction());
        menuExport_Inventory->addAction(actionExportKmlFile);
        menuWindows->addAction(actionShowProjectWindow);
        menuWindows->addAction(actionShowFilePropertiesWindow);
        menuWindows->addAction(actionShowObjectInventoryWindow);
        mainToolBar->addAction(actionNewProject);
        mainToolBar->addAction(actionOpenProject);
        mainToolBar->addAction(actionSaveProject);
        mainToolBar->addAction(actionSaveAs);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionImport);
        mainToolBar->addAction(actionFindObjects);

        retranslateUi(MainWindow);
        QObject::connect(actionImport, SIGNAL(triggered()), MainWindow, SLOT(actionImport()));
        QObject::connect(actionQuit, SIGNAL(triggered()), MainWindow, SLOT(actionQuit()));
        QObject::connect(actionAbout, SIGNAL(triggered()), MainWindow, SLOT(actionAbout()));
        QObject::connect(actionFindObjects, SIGNAL(triggered()), MainWindow, SLOT(actionFindObjects()));
        QObject::connect(actionNewProject, SIGNAL(triggered()), MainWindow, SLOT(actionCreate()));
        QObject::connect(actionOpenProject, SIGNAL(triggered()), MainWindow, SLOT(actionOpen()));
        QObject::connect(actionSaveProject, SIGNAL(triggered()), MainWindow, SLOT(actionSave()));
        QObject::connect(actionSaveAs, SIGNAL(triggered()), MainWindow, SLOT(actionSaveAs()));
        QObject::connect(actionExportKmlFile, SIGNAL(triggered()), MainWindow, SLOT(actionExportKmlFile()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "OpenSidescan - by CIDCO", nullptr));
        actionImport->setText(QApplication::translate("MainWindow", "&Import Sidescan File", nullptr));
#ifndef QT_NO_TOOLTIP
        actionImport->setToolTip(QApplication::translate("MainWindow", "Import Sidescan File", nullptr));
#endif // QT_NO_TOOLTIP
        actionAbout->setText(QApplication::translate("MainWindow", "&About OpenSidescan", nullptr));
        actionQuit->setText(QApplication::translate("MainWindow", "&Quit", nullptr));
        actionFindObjects->setText(QApplication::translate("MainWindow", "Find &Objects", nullptr));
        actionShowProjectWindow->setText(QApplication::translate("MainWindow", "&Project", nullptr));
        actionShowFilePropertiesWindow->setText(QApplication::translate("MainWindow", "&File Properties", nullptr));
        actionShowObjectInventoryWindow->setText(QApplication::translate("MainWindow", "&Object Inventory", nullptr));
        actionNewProject->setText(QApplication::translate("MainWindow", "&New Project", nullptr));
#ifndef QT_NO_TOOLTIP
        actionNewProject->setToolTip(QApplication::translate("MainWindow", "New Project", nullptr));
#endif // QT_NO_TOOLTIP
        actionOpenProject->setText(QApplication::translate("MainWindow", "&Open Project", nullptr));
#ifndef QT_NO_TOOLTIP
        actionOpenProject->setToolTip(QApplication::translate("MainWindow", "Open Project", nullptr));
#endif // QT_NO_TOOLTIP
        actionSaveProject->setText(QApplication::translate("MainWindow", "&Save Project", nullptr));
#ifndef QT_NO_TOOLTIP
        actionSaveProject->setToolTip(QApplication::translate("MainWindow", "Save Project", nullptr));
#endif // QT_NO_TOOLTIP
        actionSaveAs->setText(QApplication::translate("MainWindow", "Save &Project As ...", nullptr));
#ifndef QT_NO_TOOLTIP
        actionSaveAs->setToolTip(QApplication::translate("MainWindow", "Save Project As..", nullptr));
#endif // QT_NO_TOOLTIP
        actionExportKmlFile->setText(QApplication::translate("MainWindow", "Export KML File", nullptr));
        menuFile->setTitle(QApplication::translate("MainWindow", "P&roject", nullptr));
        menuHelp->setTitle(QApplication::translate("MainWindow", "&Help", nullptr));
        menuObject_Inventory->setTitle(QApplication::translate("MainWindow", "Ob&ject Inventory", nullptr));
        menuExport_Inventory->setTitle(QApplication::translate("MainWindow", "Export Inventory", nullptr));
        menuWindows->setTitle(QApplication::translate("MainWindow", "&Windows", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
