#include <iostream>

#include <vector>
#include <utility> // for std::pair

#include <QtTest>

#include <QObject>

#include <QWidget>

#include <QToolBar>

#include <QLineEdit>
#include <QPushButton>

#include <QMenuBar>
#include <QMenu>


#include <QDialogButtonBox>

#include <QObjectList>

#include <QDebug>

#include <QMetaObject>

#include <QGuiApplication>  // To get screen size
#include <QScreen>          // To get screen size





#include "../../src/OpenSidescan/ui/mainwindow/mainwindow.h"

#include "../../src/OpenSidescan/ui/mainimage/imagetab.h"


//#define DOSKIPTESTS


// https://doc.qt.io/qt-5/qtest-overview.html

class testGUI : public QObject
{
    Q_OBJECT



public slots:

    void interactWithModalWindowActionImport();
    void timeOutOccured();

private slots:

    void initTestCase();
    void useMenuImportToLoadSidescanFile();

private:

    MainWindow * mainWindow = nullptr;
    QTimer *timerTimeOut;
};

void testGUI::timeOutOccured()
{
    // Just in case, stop the timer
    timerTimeOut->stop();

    qDebug() << tr( "'testGUI::timeOutOccured()'" );

    std::cout << "\n\n'testGUI::timeOutOccured()'\n\n" << std::endl;


    // If there is a modal widget, it must be closed to be able to continue through the test functions
    // and delete the mainWindow
    // (Deleting the mainWindow cannot be done here,
    // it must be done in the thread which created the mainWindow.)

    QWidget * modalWidget = QApplication::activeModalWidget();

    if ( modalWidget )
    {
        qDebug() << tr( "QApplication::activeModalWidget():" );
        qDebug() << tr( "modalWidget->objectName(): " ) << modalWidget->objectName();
        qDebug() << tr( "modalWidget->windowTitle(): " ) << modalWidget->windowTitle();

        std::cout << "\n\nmodalWidget: " << modalWidget << "\n" << std::endl;
        std::cout << "modalWidget->objectName(): " << modalWidget->objectName().toStdString() << std::endl;
        std::cout << "modalWidget->windowTitle(): " << modalWidget->windowTitle().toStdString() << std::endl;

        modalWidget->close();
    }

}

void testGUI::initTestCase()
{
    //Load stylesheet
    QFile f(":qdarkstyle/style.qss");
    if (!f.exists())
    {
        printf("Unable to set stylesheet, file not found\n");
    }
    else
    {
        f.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&f);
        qApp->setStyleSheet(ts.readAll());
    }

    mainWindow = nullptr;

    timerTimeOut = new QTimer( this );
    timerTimeOut->setSingleShot( true );
    connect(timerTimeOut, &QTimer::timeout, this, &testGUI::timeOutOccured );
}

void testGUI::interactWithModalWindowActionImport()
{
    qDebug() << tr( "Beginning of interactWithModalWindowActionImport()" );

    std::cout << "Beginning of interactWithModalWindowActionImport()" << std::endl;

    mainWindow->show();
    QTest::qWait(500);

    QWidget * modalWidget = QApplication::activeModalWidget();
    QVERIFY2( modalWidget, "interactWithModalWindowActionImport: modalWidget tests false");

    QVERIFY2( modalWidget->windowTitle() == tr( "Import Sidescan Files" ),
              "interactWithModalWindowActionImport: modalWidget->windowTitle() is not 'Import Sidescan Files'" );

    QLineEdit * lineEdit = modalWidget->findChild<QLineEdit*>("fileNameEdit");
    QVERIFY2( lineEdit, "interactWithModalWindowActionImport: lineEdit tests false");

    // Number of characters currently present in the QLineEdit
    int nbBackspaces = lineEdit->text().length();

    // Use backspaces to clear the current content
    for ( int count = 0; count < nbBackspaces; count++ )
        QTest::keyClick(lineEdit, Qt::Key_Backspace, Qt::NoModifier, 10 );

    mainWindow->show();
    QTest::qWait(100);

    QString filename = "\"" + tr( "../../../test/data/wrecks/plane1.xtf" )  + "\" "
                        "\"" + tr( "../../../test/data/wrecks/scotsman3.xtf" ) + "\" ";

    QTest::keyClicks(lineEdit, filename );

    QVERIFY2( lineEdit->text() == filename, "interactWithModalWindowActionImport: filename is not the same in the QLineEdit");

    // Find the button to accept and close the modal window
    // The buttons are within a QDialogButtonBox
    QDialogButtonBox *buttonBox = modalWidget->findChild<QDialogButtonBox*>("buttonBox");
    QVERIFY2( buttonBox, "interactWithModalWindowActionImport: buttonBox tests false");

    // The buttons don't have object names,
    // I have to go through the list of buttons and find the button with
    // the desired text

    QList<QAbstractButton *> listButtonBox = buttonBox->buttons();

    QString acceptButtonText = tr( "&Open" );
    QPushButton * acceptButton = nullptr;

    for (QAbstractButton *button : listButtonBox) {
        if ( button->text() == acceptButtonText )
            acceptButton = static_cast<QPushButton * >( button );
    }

    QVERIFY2( acceptButton, "interactWithModalWindowActionImport: acceptButton tests false");
    QVERIFY2( acceptButton->isEnabled(), "interactWithModalWindowActionImport: acceptButton is not enabled");

    mainWindow->show();
    QTest::qWait(500);

    // Click button to close the modal dialog
    QTest::mouseClick(acceptButton, Qt::LeftButton);

    mainWindow->show();
    QTest::qWait(500);
}


void testGUI::useMenuImportToLoadSidescanFile()
{
    qDebug() << tr( "Beginning of 'useMenuImportToLoadSidescanFile()'" );

    if ( mainWindow ) {
        delete mainWindow;
        mainWindow = nullptr;
    }

    mainWindow = new MainWindow;

    QVERIFY2( mainWindow, "useMenuImportToLoadSidescanFile: mainWindow tests false");


//    QVERIFY2( false, "useMenuImportToLoadSidescanFile: false on purpose");


    mainWindow->show();
    QTest::qWait(1000);

    // Enables focus and widget events
    QApplication::setActiveWindow( mainWindow );
    // Shortcuts won't work unless the window is active
    QVERIFY2( QTest::qWaitForWindowActive( mainWindow ), "mainWindow is not active" );

    std::cout << "mainwindow is active" << std::endl;

    // ---- Using shortkeys

    // Keyboard to Project menu
    QTest::keyClick( mainWindow, 'r', Qt::AltModifier );

    mainWindow->show();
    QTest::qWait(2000);

    std::cout << "project menu is open" << std::endl;


    QMenuBar *menuBar = mainWindow->findChild< QMenuBar * >( "menuBar" );
    QVERIFY2( menuBar, "useMenuImportToLoadSidescanFile: menuBar tests false");

    std::cout << "found menuBar" << std::endl;

    QMenu *menuFile = menuBar->findChild< QMenu * >( "menuFile" );
    QVERIFY2( menuFile, "useMenuImportToLoadSidescanFile: menuFile tests false");

    std::cout << "found menuFile" << std::endl;

    // Time out timer in case there is a failure while interacting with the modal window
    timerTimeOut->start( 30 * 1000 );

    std::cout << "start action import" << std::endl;
    QTimer::singleShot(500, this, SLOT(interactWithModalWindowActionImport() ) );

    // Keyboard for import sidescan file
    QTest::keyClick( menuFile, 'i', Qt::AltModifier );



}



QTEST_MAIN(testGUI)

#include "testGUI.moc"
