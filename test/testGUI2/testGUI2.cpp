#include <iostream>
#include <vector>
//#include <utility> // for std::pair
#include <QtTest>
//#include <QObject>
#include <QWidget>
#include <QToolBar>
//#include <QLineEdit>
//#include <QPushButton>
#include <QMenuBar>
#include <QMenu>
#include <QDialogButtonBox>
#include <QObjectList>
#include <QMetaObject> // findChild
//#include <QGuiApplication>  // To get screen size
//#include <QScreen>          // To get screen size
#include "../../src/OpenSidescan/ui/mainwindow/mainwindow.h"
#include "../../src/OpenSidescan/ui/mainimage/imagetab.h"

class testGUI: public QObject
{
    Q_OBJECT

private:
    bool myCondition()
    {
        return true;
    }
	MainWindow * mainWindow = nullptr;

private slots:
    void initTestCase()
    {
        std::cout<<"init \n";
        mainWindow = new MainWindow;
        mainWindow->show();
        QVERIFY(mainWindow);
        QTest::qWait(500);
        delete mainWindow;
    }

    void test_import_sidescanfile()
    {
        //std::cout<<"test_import_sidescanfile \n";
        mainWindow = new MainWindow;
        QMenuBar *menuBar = mainWindow->findChild< QMenuBar * >( "menuBar" );
        QMenu *menuFile = menuBar->findChild< QMenu * >( "menuFile" );
        mainWindow->show();
        
        const QWidgetList topLevelWidgets = QApplication::topLevelWidgets();
        for (QWidget *widget : topLevelWidgets)
        {
            QString s1 = widget->windowTitle();
            qDebug()<<s1;
            bool b1 = widget->isActiveWindow();
            qDebug()<<b1;
        }
        // open menu
        QTest::keyClick( mainWindow, 'r', Qt::AltModifier );  //Qt::AltModifier,	An Alt key on the keyboard is pressed.
        QTest::keyClick( menuFile, 'i', Qt::AltModifier ); // open import dialog
        qDebug()<<"test!!!";

        
        QTest::qWait(1000);

        
        
        
        delete mainWindow;
    }

    void mySecondTest()
    {
        QVERIFY(myCondition());
        QVERIFY(1 != 2);
    }

    void cleanupTestCase()
    {
        std::cout<<"Called after myFirstTest and mySecondTest. \n";
    }
};



QTEST_MAIN(testGUI) 
#include "testGUI2.moc"
