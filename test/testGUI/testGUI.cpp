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



#include <QDebug>

#include <QMetaObject>





#include "../../src/OpenSidescan/mainwindow.h"



// https://doc.qt.io/qt-5/qtest-overview.html

class testGUI : public QObject
{
    Q_OBJECT



public slots:
    void interactWithModalWindowActionImport();

    void interactWithModalWindowActionSaveAs();

    void interactWithModalWindowAlreadyAnActiveProject();

    void interactWithModalWindowToSelectProjectToOpen();

//    void interactWithModalWindowActionImportJustVerifyWindow();


    void verifyResultOfActionImportToLoadSidescanFile();

//    void InteractWithContextMenu();

    void selectFileAndVerify( int fileToSelect, std::string & filename,
                                       std::vector<std::string> & tabNames,
                                       std::vector< std::pair< std::string,std::string > > & properties );

    void timeOutOccured();

private slots:

    // The following private slots that are not treated as test functions.


    void initTestCase();        // initTestCase() will be called before the first test function is executed.

//    void initTestCase_data();   // initTestCase_data() will be called to create a global test data table.

    void cleanupTestCase();     // cleanupTestCase() will be called after the last test function was executed.

//    void init();                // init() will be called before each test function is executed.

//    void cleanup();             // cleanup() will be called after every test function.





    // Test functions


    void useMenuImportToLoadSidescanFile();
    void verifyResultOfUseMenuImportToLoadSidescanFile();


    void useToolBarActionImportToLoadSidescanFile();
    void verifyResultOfUseToolBarActionImportToLoadSidescanFileThenSaveAs();
    void cleanAfterSaveAs();

    void useToolBarActionOpenProject();
    void verifyResultOfUseToolBarActionOpenProject();

//    void afterContextMenu();

//    void finish();


private:


    MainWindow * mainWindow;

    bool interactWithModalWindowActionImportReachedTheEnd;

    bool interactWithModalWindowActionSaveAsReachedTheEnd;

    bool verifyResultOfUseToolBarActionImportToLoadSidescanFileThenSaveAsReachedTheEnd;

    bool interactWithModalWindowAlreadyAnActiveProjectReachedTheEnd;
    bool interactWithModalWindowToSelectProjectToOpenReachedTheEnd;

    bool interactWithModalWindowActionImportJustVerifyWindowReachedTheEnd;

    bool selectFileAndVerifyReachTheEnd;

    QTimer *timerTimeOut;

};



void testGUI::timeOutOccured()
{
    // Just in case, stop the timer
    timerTimeOut->stop();

    qDebug() << tr( "'testGUI::timeOutOccured()'" );

    std::cout << "\n\n" << std::endl;


    // If there is a modal widget, it must be closed to be able to continue through the test functions
    // and delete the mainWindow
    // (Deleting the mainWindow cannot be done here,
    // it must be done in the thread which created the mainWindow.)

    QWidget * modalWidget = QApplication::activeModalWidget();

    if ( modalWidget )
    {
        qDebug() << tr( "QApplication::activeModalWidget():" );

        std::cout << "\n\nmodalWidget: " << modalWidget << "\n" << std::endl;

        qDebug() << tr( "modalWidget->objectName(): " ) << modalWidget->objectName();

        qDebug() << tr( "modalWidget->windowTitle(): " ) << modalWidget->windowTitle();

        modalWidget->close();
    }

}


void testGUI::initTestCase()
{
    mainWindow = nullptr;

    interactWithModalWindowActionImportReachedTheEnd = false;

    interactWithModalWindowActionSaveAsReachedTheEnd = false;

    verifyResultOfUseToolBarActionImportToLoadSidescanFileThenSaveAsReachedTheEnd = false;

    interactWithModalWindowAlreadyAnActiveProjectReachedTheEnd = false;
    interactWithModalWindowToSelectProjectToOpenReachedTheEnd = false;

    interactWithModalWindowActionImportJustVerifyWindowReachedTheEnd = false;

    timerTimeOut = new QTimer( this );
    timerTimeOut->setSingleShot( true );
    connect(timerTimeOut, &QTimer::timeout, this, &testGUI::timeOutOccured );
}

void testGUI::cleanupTestCase()
{

    std::cout << "\n\n\nBeginning of 'testGUI::cleanupTestCase()'" << std::endl;

    if ( mainWindow ) {
        delete mainWindow;
        mainWindow = nullptr;
    }

}


// Test functions


void testGUI::useMenuImportToLoadSidescanFile()
{
//    QSKIP( "Skip the first test" );

    qDebug() << tr( "Beginning of 'useMenuImportToLoadSidescanFile()'" );

    // setup for the test
    interactWithModalWindowActionImportReachedTheEnd = false;

    if ( mainWindow ) {
        delete mainWindow;
        mainWindow = nullptr;
    }

    mainWindow = new MainWindow;

    QVERIFY2( mainWindow, "useMenuImportToLoadSidescanFile: mainWindow tests false");

    // Enables focus and widget events
    QApplication::setActiveWindow( mainWindow );

    mainWindow->show();
    QTest::qWait(1000);

    // ---- Using shortkeys

    // Keyboard to Project menu
    QTest::keyClick( mainWindow, 'r', Qt::AltModifier );

    mainWindow->show();
    QTest::qWait(2000);


    QMenuBar *menuBar = mainWindow->findChild< QMenuBar * >( "menuBar" );
    QVERIFY2( menuBar, "useMenuImportToLoadSidescanFile: menuBar tests false");

    QMenu *menuFile = menuBar->findChild< QMenu * >( "menuFile" );
    QVERIFY2( menuFile, "useMenuImportToLoadSidescanFile: menuFile tests false");


    // Time out timer in case there is a failure while interacting with the modal window
    timerTimeOut->start( 10 * 1000 );

    QTimer::singleShot(500, this, SLOT(interactWithModalWindowActionImport() ) );

    // Keyboard for import sidescan file
    QTest::keyClick( menuFile, 'i', Qt::AltModifier );



}

//void testGUI::interactWithModalWindowActionImportJustVerifyWindow()
//{
//    qDebug() << tr( "Beginning of interactWithModalWindowActionImportJustVerifyWindow()" );

//    mainWindow->show();
//    QTest::qWait(2500);

//    QWidget * modalWidget = QApplication::activeModalWidget();
//    QVERIFY2( modalWidget, "interactWithModalWindowActionImport: modalWidget tests false");

//    QVERIFY2( modalWidget->windowTitle() == tr( "Import Sidescan Files" ),
//              "interactWithModalWindowActionImport: modalWidget->windowTitle() is not 'Import Sidescan Files'" );


//    // Find the button to cancel the modal window

//    // The buttons are within a QDialogButtonBox

//    QDialogButtonBox *buttonBox = modalWidget->findChild<QDialogButtonBox*>("buttonBox");
//    QVERIFY2( buttonBox, "interactWithModalWindowActionImport: buttonBox tests false");


//    // The buttons don't have object names,
//    // I have to go through the list of buttons and find the button with
//    // the desired text

//    QList<QAbstractButton *> listButtonBox = buttonBox->buttons();

//    QString cancelButtonText = tr( "&Cancel" );
//    QPushButton * cancelButton = nullptr;

//    for (QAbstractButton *button : listButtonBox) {

//        if ( button->text() == cancelButtonText )
//            cancelButton = static_cast<QPushButton * >( button );
//    }

//    QVERIFY2( cancelButton, "interactWithModalWindowActionImport: acceptButton tests false");
//    QVERIFY2( cancelButton->isEnabled(), "interactWithModalWindowActionImport: acceptButton is not enabled");



////    std::cout << "\n\n" << std::endl;


//    mainWindow->show();
//    QTest::qWait(500);

//    // Click button to close the modal dialog
//    QTest::mouseClick(cancelButton, Qt::LeftButton);

//    mainWindow->show();
//    QTest::qWait(500);


//    interactWithModalWindowActionImportJustVerifyWindowReachedTheEnd = true;

//}




void testGUI::verifyResultOfUseMenuImportToLoadSidescanFile()
{

    timerTimeOut->stop();

    std::cout << "\n" << std::endl;

    qDebug() << tr( "Beginning of 'testGUI::verifyResultOfUseMenuImportToLoadSidescanFile'" );


    verifyResultOfActionImportToLoadSidescanFile();


//    QVERIFY2( interactWithModalWindowActionImportJustVerifyWindowReachedTheEnd,
//                "verifyResultOfUseMenuImportToLoadSidescanFile: interactWithModalWindowActionImportJustVerifyWindowReachedTheEnd is false");

    if ( mainWindow ) {
        delete mainWindow;
        mainWindow = nullptr;
    }

}


void testGUI::verifyResultOfActionImportToLoadSidescanFile()
{

    timerTimeOut->stop();

    std::cout << "\n" << std::endl;

    qDebug() << tr( "Beginning of 'testGUI::verifyResultOfUseToolBarActionImportToLoadSidescanFileThenSaveAs'" );


    if ( interactWithModalWindowActionImportReachedTheEnd == false )
    {
        // Give time for the window to be closed after the instruction to close is sent
        // in the function responding to the timer
        QTest::qWait( 200 );

        if ( mainWindow ) {
            delete mainWindow;
            mainWindow = nullptr;
        }

        QVERIFY2( interactWithModalWindowActionImportReachedTheEnd,
                    "verifyResultOfUseToolBarActionImportToLoadSidescanFileThenSaveAs: interactWithModalWindowActionImportReachedTheEnd is false");
    }


    QVERIFY2( mainWindow, "verifyResultOfUseToolBarActionImportToLoadSidescanFileThenSaveAs: mainWindow tests false");


    // There should be one file in the tree model

    QVERIFY2( mainWindow->projectWindow->model->getNbFiles() ==  2,
                qPrintable( "verifyResultOfUseToolBarActionImportToLoadSidescanFileThenSaveAs: the number of files in the projectWindow is "
                + QString::number( mainWindow->projectWindow->model->getNbFiles() )
                + " instead of 2") );


    // Select the files and verify

    int indexFileToSelect;
    std::string filename;
    std::vector<std::string> tabNames;
    std::vector< std::pair< std::string,std::string > > propertiesToVerify;

    indexFileToSelect = 0;

    filename = "plane1.xtf";

    tabNames.clear();
    tabNames.push_back( "Channel 0" );
    tabNames.push_back( "Channel 1" );
    tabNames.push_back( "Channel 2" );

    propertiesToVerify.clear();
    propertiesToVerify.push_back( std::make_pair( "Channels (Sonar)", "3" ) );
    propertiesToVerify.push_back( std::make_pair( "Recording Program Name", "DAT2XTF" ) );

    selectFileAndVerify( indexFileToSelect, filename, tabNames, propertiesToVerify );

    mainWindow->show();
    QTest::qWait(1000);


    indexFileToSelect = 1;

    filename = "scotsman3.xtf";

    tabNames.clear();
    tabNames.push_back( "port" );
    tabNames.push_back( "starboard" );

    propertiesToVerify.clear();
    propertiesToVerify.push_back( std::make_pair( "Channels (Sonar)", "2" ) );
    propertiesToVerify.push_back( std::make_pair( "System Type", "1" ) );

    selectFileAndVerify( indexFileToSelect, filename, tabNames, propertiesToVerify );


    mainWindow->show();
    QTest::qWait(3000);

//    QVERIFY2( false, "verifyResultOfActionImportToLoadSidescanFile: false on purpose");


}


void testGUI::useToolBarActionImportToLoadSidescanFile()
{
//    QSKIP( "Skip the first test" );

    qDebug() << tr( "Beginning of 'useToolBarActionImportToLoadSidescanFile()'" );

    // setup for the test
    interactWithModalWindowActionImportReachedTheEnd = false;

    if ( mainWindow ) {
        delete mainWindow;
        mainWindow = nullptr;
    }

    mainWindow = new MainWindow;

    QVERIFY2( mainWindow, "useToolBarActionImportToLoadSidescanFile: mainWindow tests false");


    // Get action for importing a sidescan file

    QAction * actionImport = mainWindow->findChild< QAction * >( "actionImport" );
    QVERIFY2( actionImport, "useToolBarActionImportToLoadSidescanFile: actionImport tests false");


    mainWindow->show();
    QTest::qWait(1200);



    QToolBar * mainToolBar = mainWindow->findChild< QToolBar * >( "mainToolBar" );
    QVERIFY2( mainToolBar, "useToolBarActionImportToLoadSidescanFile: mainToolBar tests false");


    QWidget *widgetForActionImport = mainToolBar->widgetForAction( actionImport );
    QVERIFY2( widgetForActionImport, "useToolBarActionImportToLoadSidescanFile: widgetForActionImport tests false");


    // Time out timer in case there is a failure while interacting with the modal window
    timerTimeOut->start( 30 * 1000 ); // Time large enough to include the time it takes to load the files

    // Single shot timer for function that will interact with the modal window
    QTimer::singleShot(500, this, SLOT(interactWithModalWindowActionImport() ) );

    // Click the button to open the modal dialog
    QTest::mouseClick(widgetForActionImport, Qt::LeftButton);

}

void testGUI::interactWithModalWindowActionImport()
{
    qDebug() << tr( "Beginning of interactWithModalWindowActionImport()" );

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


    // Path with respect to the application executable
    // There may be issues, see https://doc.qt.io/qt-5/qcoreapplication.html#applicationDirPath

    QString filename = "\"" + QCoreApplication::applicationDirPath() + "/../"
                                + tr( "data/wrecks/plane1.xtf" ) + "\" "
                       "\"" + QCoreApplication::applicationDirPath() + "/../"
                                + tr( "data/wrecks/scotsman3.xtf" ) + "\" ";


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



//    std::cout << "\n\n" << std::endl;

//    QVERIFY2( false, "interactWithModalWindowActionImport: false on purpose");


    mainWindow->show();
    QTest::qWait(500);

    // Click button to close the modal dialog
    QTest::mouseClick(acceptButton, Qt::LeftButton);

    mainWindow->show();
    QTest::qWait(500);


    interactWithModalWindowActionImportReachedTheEnd = true;

}





void testGUI::verifyResultOfUseToolBarActionImportToLoadSidescanFileThenSaveAs()
{
//    QSKIP( "Skip the first test" );


    verifyResultOfActionImportToLoadSidescanFile();


    // -----------------------------------------------------------------------------------------
    // Save As project

    // Get action for project save as

    QAction * actionSaveAs = mainWindow->findChild< QAction * >( "actionSaveAs" );
    QVERIFY2( actionSaveAs, "verifyResultOfUseToolBarActionImportToLoadSidescanFileThenSaveAs: actionSaveAs tests false");


    mainWindow->show();
    QTest::qWait(1200);


    QToolBar * mainToolBar = mainWindow->findChild< QToolBar * >( "mainToolBar" );
    QVERIFY2( mainToolBar, "verifyResultOfUseToolBarActionImportToLoadSidescanFileThenSaveAs: mainToolBar tests false");


    QWidget *widgetForActionSaveAs = mainToolBar->widgetForAction( actionSaveAs );
    QVERIFY2( widgetForActionSaveAs, "verifyResultOfUseToolBarActionImportToLoadSidescanFileThenSaveAs: widgetForActionSaveAs tests false");


    // Time out timer in case there is a failure while interacting with the modal window
    timerTimeOut->start( 10 * 1000 );

    // Single shot timer for function that will interact with the modal window
    QTimer::singleShot(500, this, SLOT(interactWithModalWindowActionSaveAs() ) );

    // Click the button to open the modal dialog
    QTest::mouseClick(widgetForActionSaveAs, Qt::LeftButton);


    verifyResultOfUseToolBarActionImportToLoadSidescanFileThenSaveAsReachedTheEnd = true;


//    if ( mainWindow ) {
//        delete mainWindow;
//        mainWindow = nullptr;
//    }


}



void testGUI::interactWithModalWindowActionSaveAs()
{
    qDebug() << tr( "Beginning of interactWithModalWindowActionSaveAs()" );

    mainWindow->show();
    QTest::qWait(500);

    QWidget * modalWidget = QApplication::activeModalWidget();
    QVERIFY2( modalWidget, "interactWithModalWindowActionSaveAs: modalWidget tests false");

    QVERIFY2( modalWidget->windowTitle() == tr( "Sidescan Project Files" ),
              "interactWithModalWindowActionSaveAs: modalWidget->windowTitle() is not 'Sidescan Project Files'" );


    QLineEdit * lineEdit = modalWidget->findChild<QLineEdit*>("fileNameEdit");
    QVERIFY2( lineEdit, "interactWithModalWindowActionSaveAs: lineEdit tests false");


    // Number of characters currently present in the QLineEdit
    int nbBackspaces = lineEdit->text().length();

    // Use backspaces to clear the current content
    for ( int count = 0; count < nbBackspaces; count++ )
        QTest::keyClick(lineEdit, Qt::Key_Backspace, Qt::NoModifier, 10 );


    mainWindow->show();
    QTest::qWait(100);


    // Path with respect to the application executable
    // There may be issues, see https://doc.qt.io/qt-5/qcoreapplication.html#applicationDirPath

    QString filename = QCoreApplication::applicationDirPath() + "/../testProject/AutomatedTestProject";


    QTest::keyClicks(lineEdit, filename );

    QVERIFY2( lineEdit->text() == filename, "interactWithModalWindowActionSaveAs: filename is not the same in the QLineEdit");


    // Find the button to accept and close the modal window

    // The buttons are within a QDialogButtonBox

    QDialogButtonBox *buttonBox = modalWidget->findChild<QDialogButtonBox*>("buttonBox");
    QVERIFY2( buttonBox, "interactWithModalWindowActionSaveAs: buttonBox tests false");


    // The buttons don't have object names,
    // I have to go through the list of buttons and find the button with
    // the desired text

    QList<QAbstractButton *> listButtonBox = buttonBox->buttons();

    QString saveButtonText = tr( "&Save" );
    QPushButton * saveButton = nullptr;

    for (QAbstractButton *button : listButtonBox) {

        if ( button->text() == saveButtonText )
            saveButton = static_cast<QPushButton * >( button );
    }

    QVERIFY2( saveButton, "interactWithModalWindowActionSaveAs: saveButton tests false");
    QVERIFY2( saveButton->isEnabled(), "interactWithModalWindowActionSaveAs: saveButton is not enabled");



//    std::cout << "\n\n" << std::endl;


    mainWindow->show();
    QTest::qWait(500);

    // Click button to close the modal dialog
    QTest::mouseClick(saveButton, Qt::LeftButton);

    mainWindow->show();
    QTest::qWait(500);


    interactWithModalWindowActionSaveAsReachedTheEnd = true;

}


void testGUI::cleanAfterSaveAs()
{

//    QSKIP( "Skip testGUI::cleanAfterSaveAs()" );

    timerTimeOut->stop();

    std::cout << "\n" << std::endl;

    qDebug() << tr( "Beginning of 'testGUI::cleanAfterSaveAs'" );


    QVERIFY2( verifyResultOfUseToolBarActionImportToLoadSidescanFileThenSaveAsReachedTheEnd,
                "cleanAfterSaveAs: verifyResultOfUseToolBarActionImportToLoadSidescanFileThenSaveAsReachedTheEnd is false");

    QVERIFY2( interactWithModalWindowActionSaveAsReachedTheEnd,
                "cleanAfterSaveAs: interactWithModalWindowActionSaveAsReachedTheEnd is false");


    if ( mainWindow ) {
        delete mainWindow;
        mainWindow = nullptr;
    }

}



void testGUI::useToolBarActionOpenProject()
{
//    QSKIP( "Skip useToolBarActionOpenProject()" );

    qDebug() << tr( "Beginning of 'useToolBarActionOpenProject()'" );


    QVERIFY2( verifyResultOfUseToolBarActionImportToLoadSidescanFileThenSaveAsReachedTheEnd,
                "useToolBarActionOpenProject: verifyResultOfUseToolBarActionImportToLoadSidescanFileThenSaveAsReachedTheEnd is false");

    QVERIFY2( interactWithModalWindowActionSaveAsReachedTheEnd,
                "useToolBarActionOpenProject: interactWithModalWindowActionSaveAsReachedTheEnd is false");


//    QSKIP( "Skip the second test" );


    // In case the previous test did not complete to the end
    if ( mainWindow ) {
        delete mainWindow;
        mainWindow = nullptr;
    }

    mainWindow = new MainWindow;

    QVERIFY2( mainWindow, "useToolBarActionOpenProject: mainWindow tests false");


    // Get action for importing a sidescan file

    QAction * actionOpenProject = mainWindow->findChild< QAction * >( "actionOpenProject" );
    QVERIFY2( actionOpenProject, "useToolBarActionOpenProject: actionOpenProject tests false");


    QToolBar * mainToolBar = mainWindow->findChild< QToolBar * >( "mainToolBar" );
    QVERIFY2( mainToolBar, "useToolBarActionOpenProject: mainToolBar tests false");



    QWidget *widgetForActionOpenProject = mainToolBar->widgetForAction( actionOpenProject );
    QVERIFY2( widgetForActionOpenProject, "useToolBarActionOpenProject: widgetForActionOpenProject tests false");

    // Show the mainWindow before opening the modal window
    mainWindow->show();
    QTest::qWait(200);


    // Time out timer in case there is a failure while interacting with the modal window
    timerTimeOut->start( 10 * 1000 );

    QTimer::singleShot(500, this, SLOT(interactWithModalWindowAlreadyAnActiveProject() ) );

    // Click the button to open the modal dialog
    QTest::mouseClick(widgetForActionOpenProject, Qt::LeftButton);

}


void testGUI::interactWithModalWindowAlreadyAnActiveProject()
{
    qDebug() << tr( "Beginning of interactWithModalWindowAlreadyAnActiveProject()" );

    mainWindow->show();
    QTest::qWait(500);

    qDebug() << tr( "After starting AlreadyAnActiveProject modal window" );

//    QVERIFY2( false, "interactWithModalWindowAlreadyAnActiveProject: false on purpose");


    QWidget * modalWidget = QApplication::activeModalWidget();
    QVERIFY2( modalWidget, "interactWithModalWindowAlreadyAnActiveProject: modalWidget tests false");

//    std::cout << "\n\nmodalWidget: " << modalWidget << "\n" << std::endl;

//    qDebug() << tr( "modalWidget->objectName(): " ) << modalWidget->objectName();

//    qDebug() << tr( "modalWidget->windowTitle(): " ) << modalWidget->windowTitle();



    QDialogButtonBox *buttonBox = modalWidget->findChild<QDialogButtonBox*>("qt_msgbox_buttonbox");
    QVERIFY2( buttonBox, "interactWithModalWindowAlreadyAnActiveProject: buttonBox tests false");

    QList<QAbstractButton *> listButtonBox = buttonBox->buttons();

    QString OKButtonText = tr( "&OK" );
    QPushButton * buttonOK = nullptr;

    for (QAbstractButton *button : listButtonBox) {
        if ( button->text() == OKButtonText )
            buttonOK = static_cast<QPushButton * >( button );
    }

    QVERIFY2( buttonOK, "interactWithModalWindowAlreadyAnActiveProject: buttonOK tests false");

    // Time out timer in case there is a failure while interacting with the modal window
    timerTimeOut->start( 30 * 1000 ); // Time large enough to include the time it takes to load the files

    QTimer::singleShot(500, this, SLOT(interactWithModalWindowToSelectProjectToOpen() ) );


    // Click the button to open the modal dialog
    QTest::mouseClick(buttonOK, Qt::LeftButton);

    interactWithModalWindowAlreadyAnActiveProjectReachedTheEnd = true;


}



void testGUI::interactWithModalWindowToSelectProjectToOpen()
{

    std::cout << "\n" << std::endl;

    qDebug() << tr( "Beginning of 'testGUI::interactWithModalWindowToSelectProjectToOpen'" );


    if ( interactWithModalWindowAlreadyAnActiveProjectReachedTheEnd == false )
    {
        // Just in case, stop the timer
        timerTimeOut->stop();

        // Give time for the window to be closed after the instruction to close is sent
        // in the function responding to the timer
        QTest::qWait( 200 );

        if ( mainWindow ) {
            delete mainWindow;
            mainWindow = nullptr;
        }

        QVERIFY2( interactWithModalWindowAlreadyAnActiveProjectReachedTheEnd,
                    "interactWithModalWindowToSelectProjectToOpen: interactWithModalWindowAlreadyAnActiveProjectReachedTheEnd is false");
    }

//    QVERIFY2( mainWindow, "interactWithModalWindowToSelectProjectToOpen: mainWindow tests false");



    mainWindow->show();
    QTest::qWait(1200);

//    QVERIFY2( false, "interactWithModalWindowToSelectProjectToOpen: false on purpose");


    QWidget * modalWidget = QApplication::activeModalWidget();
    QVERIFY2( modalWidget, "interactWithModalWindowToSelectProjectToOpen: modalWidget tests false");

    QVERIFY2( modalWidget->windowTitle() == tr( "Sidescan Project Files" ),
              "interactWithModalWindowToSelectProjectToOpen: modalWidget->windowTitle() is not 'Sidescan Project Files'" );


    QLineEdit *lineEdit = modalWidget->findChild<QLineEdit*>("fileNameEdit");
    QVERIFY2( lineEdit, "interactWithModalWindowToSelectProjectToOpen: lineEdit tests false");


    // Number of characters currently present in the QLineEdit
    int nbBackspaces = lineEdit->text().length();

    // Use backspaces to clear the current content
    for ( int count = 0; count < nbBackspaces; count++ )
        QTest::keyClick(lineEdit, Qt::Key_Backspace, Qt::NoModifier, 10 );


    mainWindow->show();
    QTest::qWait(100);


    // Path with respect to the application executable
    // There may be issues, see https://doc.qt.io/qt-5/qcoreapplication.html#applicationDirPath
//    QString filename = QCoreApplication::applicationDirPath() + "/../"
//                                        + tr( "testProject/TestProject5Files.ssp" );

    QString filename = QCoreApplication::applicationDirPath() + "/../testProject/AutomatedTestProject.ssp";

    QTest::keyClicks(lineEdit, filename );

    QVERIFY2( lineEdit->text() == filename, "interactWithModalWindowToSelectProjectToOpen: filename is not the same in the QLineEdit");


    // Find the button to accept and close the modal window

    // The buttons are within a QDialogButtonBox

    QDialogButtonBox *buttonBox = modalWidget->findChild<QDialogButtonBox*>("buttonBox");
    QVERIFY2( buttonBox, "interactWithModalWindowToSelectProjectToOpen: buttonBox tests false");

    QList<QAbstractButton *> listButtonBox = buttonBox->buttons();

    QString acceptButtonText = tr( "&Open" );
    QPushButton * acceptButton = nullptr;

    for (QAbstractButton *button : listButtonBox) {

        if ( button->text() == acceptButtonText )
            acceptButton = static_cast<QPushButton * >( button );
    }

    QVERIFY2( acceptButton, "interactWithModalWindowToSelectProjectToOpen: acceptButton tests false");
    QVERIFY2( acceptButton->isEnabled(), "interactWithModalWindowToSelectProjectToOpen: acceptButton is not enabled");


    mainWindow->show();
    QTest::qWait(500);

    // Click button to close the modal dialog
    QTest::mouseClick(acceptButton, Qt::LeftButton);

    mainWindow->show();
    QTest::qWait(500);

    interactWithModalWindowToSelectProjectToOpenReachedTheEnd = true;

}


void testGUI::verifyResultOfUseToolBarActionOpenProject()
{
//    QSKIP( "Skip verifyResultOfUseToolBarActionOpenProject()" );

    QVERIFY2( verifyResultOfUseToolBarActionImportToLoadSidescanFileThenSaveAsReachedTheEnd,
                "useToolBarActionOpenProject: verifyResultOfUseToolBarActionImportToLoadSidescanFileThenSaveAsReachedTheEnd is false");

    QVERIFY2( interactWithModalWindowActionSaveAsReachedTheEnd,
                "useToolBarActionOpenProject: interactWithModalWindowActionSaveAsReachedTheEnd is false");

    timerTimeOut->stop();

    std::cout << "\n" << std::endl;

    std::cout << "\n\nBeginning of testGUI::verifyResultOfUseToolBarActionOpenProject()\n" << std::endl;

    if ( interactWithModalWindowToSelectProjectToOpenReachedTheEnd == false )
    {
        // Give time for the window to be closed after the instruction to close is sent
        // in the function responding to the timer
        QTest::qWait( 200 );

        if ( mainWindow ) {
            delete mainWindow;
            mainWindow = nullptr;
        }

        QVERIFY2( interactWithModalWindowToSelectProjectToOpenReachedTheEnd,
                    "verifyResultOfUseToolBarActionOpenProject: interactWithModalWindowActionImportReachedTheEnd is false");
    }

    QVERIFY2( mainWindow, "verifyResultOfUseToolBarActionOpenProject: mainWindow tests false");

    // There should be five files in the tree model
    QVERIFY2( mainWindow->projectWindow->model->getNbFiles() ==  2,
              qPrintable( "verifyResultOfUseToolBarActionOpenProject: the number of files in the projectWindow is "
              + QString::number( mainWindow->projectWindow->model->getNbFiles() )
              + " instead of 2") );

    mainWindow->show();
    QTest::qWait(1000);


    int indexFileToSelect;
    std::string filename;
    std::vector<std::string> tabNames;
    std::vector< std::pair< std::string,std::string > > propertiesToVerify;

    indexFileToSelect = 1;

    filename = "scotsman3.xtf";

    tabNames.clear();
    tabNames.push_back( "port" );
    tabNames.push_back( "starboard" );

    propertiesToVerify.clear();
    propertiesToVerify.push_back( std::make_pair( "Channels (Sonar)", "2" ) );
    propertiesToVerify.push_back( std::make_pair( "System Type", "1" ) );

    selectFileAndVerify( indexFileToSelect, filename, tabNames, propertiesToVerify );

    mainWindow->show();
    QTest::qWait(1000);


    indexFileToSelect = 0;

    filename = "plane1.xtf";

    tabNames.clear();
    tabNames.push_back( "Channel 0" );
    tabNames.push_back( "Channel 1" );
    tabNames.push_back( "Channel 2" );

    propertiesToVerify.clear();
    propertiesToVerify.push_back( std::make_pair( "Channels (Sonar)", "3" ) );
    propertiesToVerify.push_back( std::make_pair( "Recording Program Name", "DAT2XTF" ) );

    selectFileAndVerify( indexFileToSelect, filename, tabNames, propertiesToVerify );

    mainWindow->show();
    QTest::qWait(1000);




    if ( mainWindow )
    {
        delete mainWindow;
        mainWindow = nullptr;
    }


}



void testGUI::selectFileAndVerify( int fileToSelect, std::string & filename,
                                   std::vector<std::string> & tabNames,
                                   std::vector< std::pair< std::string,std::string > > & properties )
{

    selectFileAndVerifyReachTheEnd = false;


    // Select the file to be sure it is displayed

    QModelIndex indexFileToSelect = mainWindow->projectWindow->model->getModelIndexFileIndex( fileToSelect );

    // Scroll until it is visible
    mainWindow->projectWindow->tree->scrollTo( indexFileToSelect );

    QRect rectFileToSelect = mainWindow->projectWindow->tree->visualRect( indexFileToSelect );

    // Verify that the rectangle position corresponds to the same index in the model
    QModelIndex indexForPosition = mainWindow->projectWindow->tree->indexAt(
                                    rectFileToSelect.center() );

    QVERIFY2( indexFileToSelect == indexForPosition,
              qPrintable( "testGUI::selectFileAndVerify: fileToSelect " + QString::number( fileToSelect )
                          + ":, indexFileToSelect is different from indexForPosition" ) );



    // Select the file
    QTest::mouseClick(mainWindow->projectWindow->tree->viewport(), Qt::LeftButton,
                      Qt::NoModifier,
                      rectFileToSelect.center() );


    QModelIndex currentIndex = mainWindow->projectWindow->tree->currentIndex();
    QVERIFY2( currentIndex.row() == fileToSelect,
              qPrintable( "testGUI::selectFileAndVerify: fileToSelect " + QString::number( fileToSelect )
                          + ":, currentIndex.row() is different from fileToSelect" ) );

    std::string modelFilename = mainWindow->projectWindow->model->data(currentIndex, Qt::DisplayRole).toString().toStdString();

    QVERIFY2( modelFilename == filename,
              qPrintable( "testGUI::selectFileAndVerify: filename for fileToSelect "
            + QString::number( fileToSelect ) + " has wrong text of '" +  tr( modelFilename.c_str() )
            + "' instead of the expected '" + tr( filename.c_str() ) + "'" ) );



//    std::cout << "\n\ncurrentIndex.row(): " << currentIndex.row() << std::endl;

    // Give a bit of time to be sure the tabs are settled
    mainWindow->show();
    QTest::qWait(100);

    // Verify tabs

    QVERIFY2( mainWindow->tabs,
              qPrintable( "testGUI::selectFileAndVerify: fileToSelect "
            + QString::number( fileToSelect ) + ", mainWindow->tabs tests false" ) );

    QVERIFY2( mainWindow->tabs->count() == tabNames.size(),
              qPrintable( "testGUI::selectFileAndVerify: fileToSelect "
            + QString::number( fileToSelect ) + ", the number of tabs is "
            + QString::number( mainWindow->tabs->count() ) + " instead of the expected "
            + QString::number( tabNames.size() ) ) );


    for ( int count = 0; count < tabNames.size(); count++ )
        QVERIFY2( mainWindow->tabs->tabText( count ).toStdString() == tabNames[ count ],
                    qPrintable( "testGUI::selectFileAndVerify: fileToSelect "
                                + QString::number( fileToSelect ) + ", tab with index "
                  + QString::number( count ) + " has wrong tabText of '" +  mainWindow->tabs->tabText( count )
                  + "' instead of the expected '" + tr( tabNames[ count ].c_str() ) + "'" ) );


    // Verify that the first tab is selected

    QVERIFY2( mainWindow->tabs->currentIndex() == 0,
                qPrintable( "testGUI::selectFileAndVerify: fileToSelect "
              + QString::number( fileToSelect ) + ", The current tab index is different from zero") );


    QTabBar *tabBar = mainWindow->tabs->tabBar();

    QVERIFY2( tabBar,
              qPrintable( "testGUI::selectFileAndVerify: fileToSelect "
            + QString::number( fileToSelect ) + ", tabBar tests false" ) );


    QVERIFY2( tabBar->count() == tabNames.size(),
              qPrintable( "testGUI::selectFileAndVerify: fileToSelect "
            + QString::number( fileToSelect ) + ", tabBar->count() is "
            + QString::number( tabBar->count() ) + " instead of the expected "
            + QString::number( tabNames.size() ) ) );


    // Click on all the tabs in reverse order
    for ( int count = tabNames.size() - 1; count >= 0 ; count-- ) {

        const QPoint tabPos = tabBar->tabRect( count ).center();
        QTest::mouseClick( tabBar, Qt::LeftButton, {}, tabPos);

        mainWindow->show();
        QTest::qWait(500);

        QVERIFY2( mainWindow->tabs->currentIndex() == count,
                  qPrintable( "testGUI::selectFileAndVerify: fileToSelect "
                + QString::number( fileToSelect ) + ", the current tab index of "
                + QString::number( mainWindow->tabs->currentIndex() ) + " is different from the target of "
                + QString::number( count ) ) );
    }


    // Verifying file properties (which uses QTableWidget class)

    for ( int count = 0; count < properties.size(); count++ ) {

        QList<QTableWidgetItem *> items = mainWindow->fileInfo->propertiesTable->findItems(
                                                    QString( properties[count].first.c_str() ), Qt::MatchExactly);

        // It should find the item only once
        QVERIFY2( items.size() == 1,
                  qPrintable( "testGUI::selectFileAndVerify: fileToSelect "
                + QString::number( fileToSelect ) + ", findItems of '"
                + QString( properties[count].first.c_str() ) + "' found " + QString::number( items.size() )
                + " match(es), it should be 1" ) );


        QVERIFY2( items[ 0 ]->column() == 0,
                qPrintable( "testGUI::selectFileAndVerify: fileToSelect "
              + QString::number( fileToSelect ) + ", item of '"
              + QString( properties[count].first.c_str() ) + "' was found in column "
              + QString::number( items[ 0 ]->column() )
              + ". It should be in column 0" ) );


        std::string propertyText = mainWindow->fileInfo->propertiesTable->item( items[ 0 ]->row(), 1 )->data( Qt::DisplayRole ).toString().toStdString();
        QVERIFY2( propertyText == properties[count].second,
                  qPrintable( "testGUI::selectFileAndVerify: fileToSelect "
                + QString::number( fileToSelect ) + ", property '"
                + QString( properties[count].first.c_str() )
                + "', corresponding property value text is '"
                + QString( propertyText.c_str() ) + "' while it should be '"
                + QString( properties[count].second.c_str() ) + "'" ) );

    }


    selectFileAndVerifyReachTheEnd = true;

}



//void testGUI::afterContextMenu()
//{
//    qDebug() << tr( "Beginning of 'testGUI::afterContextMenu()'" );

//    mainWindow->show();
//    QTest::qWait(10000);

//    if ( mainWindow )
//    {
//        delete mainWindow;
//        mainWindow = nullptr;
//    }


//}


//void testGUI::InteractWithContextMenu()
//{

//    qDebug() << tr( "Beginning of 'testGUI::InteractWithContextMenu'" );


//    mainWindow->show();
//    QTest::qWait(5000);

//}




























QTEST_MAIN(testGUI)

#include "testGUI.moc"
