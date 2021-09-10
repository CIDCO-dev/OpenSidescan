#include <iostream>
#include <vector>
//#include <utility> // for std::pair
#include <QtTest>
//#include <QObject>
#include <QWidget>
//#include <QToolBar>
//#include <QLineEdit>
//#include <QPushButton>
//#include <QMenuBar>
//#include <QMenu>
//#include <QDialogButtonBox>
//#include <QObjectList>
//#include <QMetaObject>
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
    }

    void myFirstTest()
    {
        QVERIFY(true); // check that a condition is satisfied
        QCOMPARE(1, 1); // compare two values
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



QTEST_MAIN(testGui)
#include "initTestCase.moc"
#include "myFirstTest.moc"
#include "mySecondTest.moc"
#include "cleanupTestCase.moc" 