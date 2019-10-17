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
    QAction *actionOpen;
    QAction *actionAbout;
    QAction *actionQuit;
    QAction *actionFindObjects;
    QAction *actionShowProjectWindow;
    QAction *actionShowFilePropertiesWindow;
    QAction *actionShowObjectInventoryWindow;
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuHelp;
    QMenu *menuObject_Inventory;
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
        actionOpen = new QAction(MainWindow);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/Images/resources/open.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionOpen->setIcon(icon1);
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
        menuFile->addAction(actionOpen);
        menuFile->addSeparator();
        menuFile->addAction(actionQuit);
        menuHelp->addAction(actionAbout);
        menuObject_Inventory->addAction(actionFindObjects);
        menuWindows->addAction(actionShowProjectWindow);
        menuWindows->addAction(actionShowFilePropertiesWindow);
        menuWindows->addAction(actionShowObjectInventoryWindow);
        mainToolBar->addAction(actionOpen);
        mainToolBar->addAction(actionFindObjects);
        mainToolBar->addAction(actionAbout);

        retranslateUi(MainWindow);
        QObject::connect(actionOpen, SIGNAL(triggered()), MainWindow, SLOT(actionOpen()));
        QObject::connect(actionQuit, SIGNAL(triggered()), MainWindow, SLOT(actionQuit()));
        QObject::connect(actionAbout, SIGNAL(triggered()), MainWindow, SLOT(actionAbout()));
        QObject::connect(actionFindObjects, SIGNAL(triggered()), MainWindow, SLOT(actionFindObjects()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "OpenSidescan - by CIDCO", nullptr));
        actionOpen->setText(QApplication::translate("MainWindow", "&Open Sidescan File", nullptr));
        actionAbout->setText(QApplication::translate("MainWindow", "&About OpenSidescan", nullptr));
        actionQuit->setText(QApplication::translate("MainWindow", "&Quit", nullptr));
        actionFindObjects->setText(QApplication::translate("MainWindow", "Find &Objects", nullptr));
        actionShowProjectWindow->setText(QApplication::translate("MainWindow", "Project", nullptr));
        actionShowFilePropertiesWindow->setText(QApplication::translate("MainWindow", "File Properties", nullptr));
        actionShowObjectInventoryWindow->setText(QApplication::translate("MainWindow", "Object Inventory", nullptr));
        menuFile->setTitle(QApplication::translate("MainWindow", "&File", nullptr));
        menuHelp->setTitle(QApplication::translate("MainWindow", "&Help", nullptr));
        menuObject_Inventory->setTitle(QApplication::translate("MainWindow", "Ob&ject Inventory", nullptr));
        menuWindows->setTitle(QApplication::translate("MainWindow", "&Windows", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
