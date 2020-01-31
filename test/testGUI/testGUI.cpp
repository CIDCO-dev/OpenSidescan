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





#include "../../src/OpenSidescan/mainwindow.h"

#include "../../src/OpenSidescan/imagetab.h"


#define DOSKIPTESTS


// https://doc.qt.io/qt-5/qtest-overview.html

class testGUI : public QObject
{
    Q_OBJECT



public slots:

    void selectFileAndVerify( int fileToSelect, std::string & filename,
                                       std::vector<std::string> & tabNames,
                                       std::vector< std::pair< std::string,std::string > > & properties );

    void useMouseToFormObjectBound( int fileToSelect, std::string & filename, int tabIndex,
                                        int topLeftCornerX, int topLeftCornerY,
                                        int bottomRightCornerX, int bottomRightCornerY );

    void interactWithModalWindowActionImport();

#ifndef DOSKIPTESTS

    void verifyResultOfImportToLoadSidescanFile();




    void interactWithModalWindowActionSaveAs();

    void interactWithModalWindowAlreadyAnActiveProject();



    void interactWithModalWindowToSelectProjectToOpen();

    void interactWithModalWindowToFindObjects();

    void interactWithModalWindowToExportKMLfile();

#endif


    void interactWithModalWindowObjectInformation();



//    void InteractWithContextMenu();



    void timeOutOccured();

private slots:

    // The following private slots that are not treated as test functions.


    void initTestCase();        // initTestCase() will be called before the first test function is executed.

//    void initTestCase_data();   // initTestCase_data() will be called to create a global test data table.

    void cleanupTestCase();     // cleanupTestCase() will be called after the last test function was executed.

//    void init();                // init() will be called before each test function is executed.

//    void cleanup();             // cleanup() will be called after every test function.





    // Test functions


#ifndef DOSKIPTESTS


    // Load sidescan files using menubar Import and test toolbar New Project
    void useMenuImportToLoadSidescanFile();
    void verifyResultOfUseMenuImportToLoadSidescanFile();
    void cleanupIfVerifyResultOfUseMenuImportToLoadSidescanFileDidNotReachedTheEnd();
    void useToolBarActionNewProject();
    void verifyResultOfUseToolBarActionNewProject();
    void cleanAfterToolBarActionNewProject();

    // Load sidescan files using toolbar Import and save project with toolbar Save Project As
    void useToolBarActionImportToLoadSidescanFile();
    void verifyResultOfUseToolBarActionImportToLoadSidescanFileThenSaveAs();
    void cleanAfterSaveAs();

    // Open saved project using toolbar Open Project
    void useToolBarActionOpenProject();
    void verifyResultOfUseToolBarActionOpenProject();

    // Use toolbar Find Objects to verify that modal window opens
    void useToolBarActionFindObjects();
    void cleanAfterUseToolBarActionFindObjects();

    // Use menubar Object Inventory/Export Inventory/Export KML File to verify that modal window opens
    void useMenuExportKMLfile();
    void cleanAfterUseMenuExportKMLfile();
#endif

    // Verify using mouse to create an object, then saving the object image
    void useToolBarActionImportToLoadSidescanFileForFindingObjects();
    void operateMouseToCreateObjects();
    void cleanAfterFindingObjects();







//    void afterContextMenu();

//    void finish();


private:


    MainWindow * mainWindow;

    bool interactWithModalWindowActionImportReachedTheEnd;

    bool verifyResultOfUseMenuImportToLoadSidescanFileReachedTheEnd;

    bool verifyResultOfImportToLoadSidescanFileReachedTheEnd;

    bool interactWithModalWindowActionSaveAsReachedTheEnd;

    bool verifyResultOfUseToolBarActionImportToLoadSidescanFileThenSaveAsReachedTheEnd;

    bool interactWithModalWindowAlreadyAnActiveProjectReachedTheEnd;
    bool interactWithModalWindowToSelectProjectToOpenReachedTheEnd;

    bool interactWithModalWindowActionImportJustVerifyWindowReachedTheEnd;

    bool doInteractWithModalWindowToSelectProjectToOpen;

    bool selectFileAndVerifyReachTheEnd;

    bool interactWithModalWindowToFindObjectsReachedTheEnd;

    bool interactWithModalWindowToExportKMLfileReachedTheEnd;

    bool interactWithModalWindowObjectInformationReachedTheEnd;


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
		
        std::cout << "modalWidget->objectName(): " << modalWidget->objectName().toStdString() << std::endl;

        std::cout << "modalWidget->windowTitle(): " << modalWidget->windowTitle().toStdString() << std::endl;
		

        qDebug() << tr( "modalWidget->objectName(): " ) << modalWidget->objectName();

        qDebug() << tr( "modalWidget->windowTitle(): " ) << modalWidget->windowTitle();

        modalWidget->close();
    }

}


void testGUI::initTestCase()
{
    mainWindow = nullptr;

    interactWithModalWindowActionImportReachedTheEnd = false;

    verifyResultOfUseMenuImportToLoadSidescanFileReachedTheEnd = false;

    verifyResultOfImportToLoadSidescanFileReachedTheEnd = false;

    interactWithModalWindowActionSaveAsReachedTheEnd = false;

    verifyResultOfUseToolBarActionImportToLoadSidescanFileThenSaveAsReachedTheEnd = false;

    interactWithModalWindowAlreadyAnActiveProjectReachedTheEnd = false;
    interactWithModalWindowToSelectProjectToOpenReachedTheEnd = false;

    interactWithModalWindowActionImportJustVerifyWindowReachedTheEnd = false;

    doInteractWithModalWindowToSelectProjectToOpen = false;

    interactWithModalWindowToFindObjectsReachedTheEnd = false;

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


#ifndef DOSKIPTESTS

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


//    QVERIFY2( false, "useMenuImportToLoadSidescanFile: false on purpose");


    mainWindow->show();
    QTest::qWait(1000);

    // Enables focus and widget events
    QApplication::setActiveWindow( mainWindow );
    // Shortcuts won't work unless the window is active
    QVERIFY2( QTest::qWaitForWindowActive( mainWindow ), "mainWindow is not active" );

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
    timerTimeOut->start( 30 * 1000 );

    QTimer::singleShot(500, this, SLOT(interactWithModalWindowActionImport() ) );

    // Keyboard for import sidescan file
    QTest::keyClick( menuFile, 'i', Qt::AltModifier );



}




void testGUI::verifyResultOfUseMenuImportToLoadSidescanFile()
{

    timerTimeOut->stop();

    std::cout << "\n" << std::endl;

    qDebug() << tr( "Beginning of 'testGUI::verifyResultOfUseMenuImportToLoadSidescanFile'" );

    verifyResultOfImportToLoadSidescanFileReachedTheEnd = false;

    verifyResultOfImportToLoadSidescanFile();

    QVERIFY2( verifyResultOfImportToLoadSidescanFileReachedTheEnd,
                "verifyResultOfUseMenuImportToLoadSidescanFile: verifyResultOfImportToLoadSidescanFileReachedTheEnd is false");

    qDebug() << tr( "'testGUI::verifyResultOfUseMenuImportToLoadSidescanFile' after 'verifyResultOfImportToLoadSidescanFile()'" );

    verifyResultOfUseMenuImportToLoadSidescanFileReachedTheEnd = true;

}


void testGUI::cleanupIfVerifyResultOfUseMenuImportToLoadSidescanFileDidNotReachedTheEnd()
{
    std::cout << "\n\nBeginning of 'testGUI::cleanupIfVerifyResultOfUseMenuImportToLoadSidescanFileDidNotReachedTheEnd()'"
              << "\nverifyResultOfUseMenuImportToLoadSidescanFileReachedTheEnd: " << verifyResultOfUseMenuImportToLoadSidescanFileReachedTheEnd
              << "\n" << std::endl;


    if ( verifyResultOfUseMenuImportToLoadSidescanFileReachedTheEnd == false )
    {
        // Give time for the window to be closed after the instruction to close is sent
        // in the function responding to the timer
        QTest::qWait( 200 );

        if ( mainWindow ) {
            delete mainWindow;
            mainWindow = nullptr;
        }

        QVERIFY2( verifyResultOfUseMenuImportToLoadSidescanFileReachedTheEnd,
                    "cleanupIfVerifyResultOfUseMenuImportToLoadSidescanFileDidNotReachedTheEnd: verifyResultOfUseMenuImportToLoadSidescanFileReachedTheEnd is false");
    }

}

void testGUI::useToolBarActionNewProject()
{
    // Setup up
    interactWithModalWindowAlreadyAnActiveProjectReachedTheEnd = false;

    std::cout << "\n\nBeginning of 'testGUI::useToolBarActionNewProject()'"
              << "\nverifyResultOfUseMenuImportToLoadSidescanFileReachedTheEnd: " << verifyResultOfUseMenuImportToLoadSidescanFileReachedTheEnd
              << "\n" << std::endl;


    QVERIFY2( verifyResultOfUseMenuImportToLoadSidescanFileReachedTheEnd,
                "useToolBarActionNewProject: verifyResultOfUseMenuImportToLoadSidescanFileReachedTheEnd is false");

    QVERIFY2( mainWindow, "useToolBarActionNewProject: mainWindow tests false");


    // Get action for New Project

    QAction * actionNewProject = mainWindow->findChild< QAction * >( "actionNewProject" );
    QVERIFY2( actionNewProject, "useToolBarActionNewProject: actionNewProject tests false");


    QToolBar * mainToolBar = mainWindow->findChild< QToolBar * >( "mainToolBar" );
    QVERIFY2( mainToolBar, "useToolBarActionNewProject: mainToolBar tests false");



    QWidget *widgetForActionNewProject = mainToolBar->widgetForAction( actionNewProject );
    QVERIFY2( widgetForActionNewProject, "useToolBarActionNewProject: widgetForActionNewProject tests false");

    // Show the mainWindow before opening the modal window
    mainWindow->show();
    QTest::qWait(200);


    // Time out timer in case there is a failure while interacting with the modal window
    timerTimeOut->start( 5 * 1000 );


    doInteractWithModalWindowToSelectProjectToOpen = false;

    QTimer::singleShot(500, this, SLOT(interactWithModalWindowAlreadyAnActiveProject() ) );

    // Click the button to open the modal dialog
    QTest::mouseClick(widgetForActionNewProject, Qt::LeftButton);


}


void testGUI::verifyResultOfUseToolBarActionNewProject()
{

    qDebug() << tr( "Beginning of verifyResultOfUseToolBarActionNewProject()" );



    std::cout << "\n\nBeginning of 'testGUI::verifyResultOfUseToolBarActionNewProject'"
              << "\nverifyResultOfUseMenuImportToLoadSidescanFileReachedTheEnd: " << verifyResultOfUseMenuImportToLoadSidescanFileReachedTheEnd
              << "\n" << std::endl;

    QVERIFY2( verifyResultOfUseMenuImportToLoadSidescanFileReachedTheEnd,
                "verifyResultOfUseToolBarActionNewProject: verifyResultOfUseMenuImportToLoadSidescanFileReachedTheEnd is false");


    QVERIFY2( mainWindow, "verifyResultOfUseToolBarActionNewProject: mainWindow tests false");


    mainWindow->show();
    QTest::qWait(500);


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
                    "verifyResultOfUseToolBarActionNewProject: interactWithModalWindowAlreadyAnActiveProjectReachedTheEnd is false");
    }


    QVERIFY2( mainWindow, "verifyResultOfUseToolBarActionNewProject: mainWindow tests false");


    // There should be 0 files in the tree model
    QVERIFY2( mainWindow->projectWindow->model->getNbFiles() ==  0,
              qPrintable( "verifyResultOfUseToolBarActionNewProject: the number of files in the projectWindow is "
              + QString::number( mainWindow->projectWindow->model->getNbFiles() )
              + " instead of 0") );

    mainWindow->show();
    QTest::qWait(1000);

//    std::cout << "\n\nmainWindow->tabs: " << mainWindow->tabs << std::endl;
//    std::cout << "mainWindow->tabs->count(): " << mainWindow->tabs->count() << std::endl;

    QVERIFY2( mainWindow->tabs->count() == 0,
              qPrintable( "testGUI::verifyResultOfUseToolBarActionNewProject: the number of tabs is "
            + QString::number( mainWindow->tabs->count() ) + " instead of the expected 0" ) );


    QTabBar *tabBar = mainWindow->tabs->tabBar();

//    std::cout << "tabBar: " << tabBar << std::endl;
//    std::cout << "tabBar->count(): " << tabBar->count() << std::endl;

    QVERIFY2( tabBar->count() == 0,
              qPrintable( "testGUI::verifyResultOfUseToolBarActionNewProject: tabBar->count() is "
            + QString::number( tabBar->count() ) + " instead of the expected 0" ) );


    // For an empty table, it should have 0 rows or the function QTableWidget::item
    // should return nullptr for all its item

//    std::cout << "\n\nmainWindow->fileInfo->propertiesTables: " << mainWindow->fileInfo->propertiesTable << std::endl;
//    std::cout << "\n\nmainWindow->fileInfo->propertiesTables->rowCount(): " << mainWindow->fileInfo->propertiesTable->rowCount() << std::endl;

    bool tableIsEmpty = true;

    int nbColumns = mainWindow->fileInfo->propertiesTable->columnCount();

    for ( int row = 0; row < mainWindow->fileInfo->propertiesTable->rowCount(); row++ ) {

//        std::cout << "count: " << row << ", ";

        for ( int column = 0; column < nbColumns; column++ ) {

            if ( mainWindow->fileInfo->propertiesTable->item(row, column) != nullptr ) {

//                std::cout << "\"" << mainWindow->fileInfo->propertiesTable->item(row, 0)->text().toStdString() << "\"";

                tableIsEmpty = false;
            }
//            else {
//                std::cout << "nullptr";
//            }

//            std::cout << " ";

        }

//        std::cout << "\n" ;
    }

    QVERIFY2( tableIsEmpty, "verifyResultOfUseToolBarActionNewProject: mainWindow->fileInfo->propertiesTable is not empty");

}

void testGUI::cleanAfterToolBarActionNewProject()
{

//    QSKIP( "Skip testGUI::cleanAfterToolBarActionNewProject()" );

    timerTimeOut->stop();

    std::cout << "\n" << std::endl;

    qDebug() << tr( "Beginning of 'testGUI::cleanAfterToolBarActionNewProject'" );

    if ( mainWindow ) {
        delete mainWindow;
        mainWindow = nullptr;
    }

}


void testGUI::verifyResultOfImportToLoadSidescanFile()
{

    timerTimeOut->stop();

    std::cout << "\n" << std::endl;

    qDebug() << tr( "Beginning of 'testGUI::verifyResultOfImportToLoadSidescanFile'" );


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
                    "verifyResultOfImportToLoadSidescanFile: interactWithModalWindowActionImportReachedTheEnd is false");
    }


    QVERIFY2( mainWindow, "verifyResultOfUseToolBarActionImportToLoadSidescanFileThenSaveAs: mainWindow tests false");



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

//    QVERIFY2( false, "verifyResultOfImportToLoadSidescanFile: false on purpose");



    verifyResultOfImportToLoadSidescanFileReachedTheEnd = true;

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
    timerTimeOut->start( 50 * 1000 ); // Time large enough to include the time it takes to load the files

    // Single shot timer for function that will interact with the modal window
    QTimer::singleShot(500, this, SLOT(interactWithModalWindowActionImport() ) );

    // Click the button to open the modal dialog
    QTest::mouseClick(widgetForActionImport, Qt::LeftButton);

}

#endif
void testGUI::interactWithModalWindowActionImport()
{
    qDebug() << tr( "Beginning of interactWithModalWindowActionImport()" );

    mainWindow->show();
    QTest::qWait(500);

    QWidget * modalWidget = QApplication::activeModalWidget();
    QVERIFY2( modalWidget, "interactWithModalWindowActionImport: modalWidget tests false");

    QVERIFY2( modalWidget->windowTitle() == tr( "Import Sidescan Files" ),
              "interactWithModalWindowActionImport: modalWidget->windowTitle() is not 'Import Sidescan Files'" );


//	const QObjectList list = modalWidget->children();
	
//	std::cout << "\nmodalWidget list.size(): " << list.size() << std::endl;
	
//	for ( QObject *children : list ) {
//		std::cout << children->objectName().toStdString()
//			<< ", children: " << children->metaObject()->className() << std::endl;
//	}


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

    //
//    QString filename = "\"" + QCoreApplication::applicationDirPath() + "/../"
//                                + tr( "data/wrecks/plane1.xtf" ) + "\" "
//                       "\"" + QCoreApplication::applicationDirPath() + "/../"
//                                + tr( "data/wrecks/scotsman3.xtf" ) + "\" ";

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


//	std::cout << "interactWithModalWindowActionImport() button text: " << std::endl;

    for (QAbstractButton *button : listButtonBox) {
		
//		std::cout << "  \"" << button->text().toStdString() << "\"" << std::endl;

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


#ifndef DOSKIPTESTS


void testGUI::verifyResultOfUseToolBarActionImportToLoadSidescanFileThenSaveAs()
{
//    QSKIP( "Skip the first test" );

    verifyResultOfImportToLoadSidescanFileReachedTheEnd = false;

    verifyResultOfImportToLoadSidescanFile();

    QVERIFY2( verifyResultOfImportToLoadSidescanFileReachedTheEnd,
                "verifyResultOfUseToolBarActionImportToLoadSidescanFileThenSaveAs: verifyResultOfImportToLoadSidescanFileReachedTheEnd is false");


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
    timerTimeOut->start( 30 * 1000 );

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

//    QVERIFY2( modalWidget->windowTitle() == tr( "Sidescan Project Files" ),
//              "interactWithModalWindowActionSaveAs: modalWidget->windowTitle() is not 'Sidescan Project Files'" );

    QVERIFY2( modalWidget->windowTitle() == tr( "Save Sidescan Project" ),
              "interactWithModalWindowActionSaveAs: modalWidget->windowTitle() is not 'Save Sidescan Project'" );


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

//    QString filename = QCoreApplication::applicationDirPath() + "/../testProject/AutomatedTestProject";

    QString filename =  tr( "../../../test/testProject/AutomatedTestProject" );


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

//    QVERIFY2( false, "interactWithModalWindowActionSaveAs: false on purpose");



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


    // Setup for test
    interactWithModalWindowAlreadyAnActiveProjectReachedTheEnd = false;
    interactWithModalWindowToSelectProjectToOpenReachedTheEnd = false;


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


    // Get action for Open Project

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

    doInteractWithModalWindowToSelectProjectToOpen = true;

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

    QString OKButtonTextWithAmpersand = tr( "&OK" );
	QString OKButtonText = tr( "OK" );
    QPushButton * buttonOK = nullptr;
	
//	std::cout << "interactWithModalWindowAlreadyAnActiveProject, button text:" << std::endl;
	

    for (QAbstractButton *button : listButtonBox) {
		
//		std::cout << "  " << button->text().toStdString() << std::endl;
		
        if ( button->text() == OKButtonText || button->text() == OKButtonTextWithAmpersand )
            buttonOK = static_cast<QPushButton * >( button );
    }



    QVERIFY2( buttonOK, "interactWithModalWindowAlreadyAnActiveProject: buttonOK tests false");

    if ( doInteractWithModalWindowToSelectProjectToOpen ) {

        // Time out timer in case there is a failure while interacting with the modal window
        timerTimeOut->start( 50 * 1000 ); // Time large enough to include the time it takes to load the files

        QTimer::singleShot(500, this, SLOT(interactWithModalWindowToSelectProjectToOpen() ) );

    }


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

//    QVERIFY2( modalWidget->windowTitle() == tr( "Sidescan Project Files" ),
//              "interactWithModalWindowToSelectProjectToOpen: modalWidget->windowTitle() is not 'Sidescan Project Files'" );


    QVERIFY2( modalWidget->windowTitle() == tr( "Open Sidescan Project" ),
              "interactWithModalWindowToSelectProjectToOpen: modalWidget->windowTitle() is not 'Open Sidescan Project'" );


//	const QObjectList list = modalWidget->children();
	
//	std::cout << "\nmodalWidget list.size(): " << list.size() << std::endl;
	
//	for ( QObject *children : list ) {
//		std::cout << children->objectName().toStdString()
//			<< ", children: " << children->metaObject()->className() << std::endl;
//	}


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

//    QString filename = QCoreApplication::applicationDirPath() + "/../testProject/AutomatedTestProject.ssp";

    QString filename = tr( "../../../test/testProject/AutomatedTestProject.ssp" );


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

#endif

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

#ifndef DOSKIPTESTS

void testGUI::useToolBarActionFindObjects()
{

    std::cout << "\n\nBeginning of 'testGUI::useToolBarActionFindObjects()'"<< "\n" << std::endl;

    // Setup up
    interactWithModalWindowToFindObjectsReachedTheEnd = false;

    if ( mainWindow ) {
        delete mainWindow;
        mainWindow = nullptr;
    }

    mainWindow = new MainWindow;

    QVERIFY2( mainWindow, "useToolBarActionFindObjects: mainWindow tests false");


    // Get action for FindObjects

    QAction * actionFindObjects = mainWindow->findChild< QAction * >( "actionFindObjects" );
    QVERIFY2( actionFindObjects, "useToolBarActionFindObjects: actionFindObjects tests false");


    QToolBar * mainToolBar = mainWindow->findChild< QToolBar * >( "mainToolBar" );
    QVERIFY2( mainToolBar, "useToolBarActionFindObjects: mainToolBar tests false");



    QWidget *widgetForActionFindObjects= mainToolBar->widgetForAction( actionFindObjects );
    QVERIFY2( widgetForActionFindObjects, "useToolBarActionFindObjects: widgetForActionNewProject tests false");

    // Show the mainWindow before opening the modal window
    mainWindow->show();
    QTest::qWait(1000);


    // Time out timer in case there is a failure while interacting with the modal window
    timerTimeOut->start( 5 * 1000 );

    QTimer::singleShot(500, this, SLOT(interactWithModalWindowToFindObjects() ) );

    // Click the button to open the modal dialog
    QTest::mouseClick(widgetForActionFindObjects, Qt::LeftButton);
}

void testGUI::interactWithModalWindowToFindObjects()
{

    qDebug() << tr( "Beginning of interactWithModalWindowToFindObjects()" );

    mainWindow->show();
    QTest::qWait(500);

    QWidget * modalWidget = QApplication::activeModalWidget();
    QVERIFY2( modalWidget, "interactWithModalWindowToFindObjects: modalWidget tests false");

    QVERIFY2( modalWidget->windowTitle() == tr( "Find Objects" ),
              "interactWithModalWindowToFindObjects: modalWidget->windowTitle() is not 'Find Objects'" );


    // Find the button to Cancel the modal window

    // The buttons are within a QDialogButtonBox

    QDialogButtonBox *buttonBox = modalWidget->findChild<QDialogButtonBox*>("buttonBox");
    QVERIFY2( buttonBox, "interactWithModalWindowToFindObjects: buttonBox tests false");


    // The buttons don't have object names,
    // I have to go through the list of buttons and find the button with
    // the desired text

    QList<QAbstractButton *> listButtonBox = buttonBox->buttons();

    // QString cancelButtonText = tr( "&Cancel" );
    // QPushButton * cancelButton = nullptr;

    // QString okButtonText = tr( "&OK" );
    // QPushButton * okButton = nullptr;
	
    QString cancelButtonTextWithAmpersand = tr( "&Cancel" );
    QString cancelButtonText = tr( "Cancel" );
    QPushButton * cancelButton = nullptr;

    QString okButtonTextWithAmpersand = tr( "&OK" );
    QString okButtonText = tr( "OK" );
    QPushButton * okButton = nullptr;	

    for (QAbstractButton *button : listButtonBox) {

        if ( button->text() == cancelButtonText || button->text() == cancelButtonTextWithAmpersand )
            cancelButton = static_cast<QPushButton * >( button );
        else if ( button->text() == okButtonText || button->text() == okButtonTextWithAmpersand )
            okButton = static_cast<QPushButton * >( button );
    }

//    QVERIFY2( false, "interactWithModalWindowToFindObjects: false on purpose");

    QVERIFY2( cancelButton, "interactWithModalWindowToFindObjects: cancelButton tests false");
    QVERIFY2( cancelButton->isEnabled(), "interactWithModalWindowToFindObjects: cancelButton is not enabled");

    QVERIFY2( okButton, "interactWithModalWindowToFindObjects: okButton tests false");
    QVERIFY2( okButton->isEnabled(), "interactWithModalWindowToFindObjects: okButton is not enabled");


    mainWindow->show();
    QTest::qWait(500);

    // Click cancel button to close the modal dialog without finding objects
    QTest::mouseClick(cancelButton, Qt::LeftButton);

    mainWindow->show();
    QTest::qWait(1000);


    interactWithModalWindowToFindObjectsReachedTheEnd = true;

}



void testGUI::cleanAfterUseToolBarActionFindObjects()
{
    std::cout << "\n\n\nBeginning of 'testGUI::cleanAfterUseToolBarActionFindObjects()'" << std::endl;


    QVERIFY2( interactWithModalWindowToFindObjectsReachedTheEnd, "cleanAfterUseToolBarActionFindObjects: interactWithModalWindowToFindObjectsReachedTheEnd is false" );


    if ( mainWindow ) {
        delete mainWindow;
        mainWindow = nullptr;
    }
}




void testGUI::useMenuExportKMLfile()
{

    std::cout << "\n\nBeginning of 'testGUI::useMenuExportKMLfile()'"<< "\n" << std::endl;

    // Setup
    interactWithModalWindowToExportKMLfileReachedTheEnd = false;

    if ( mainWindow ) {
        delete mainWindow;
        mainWindow = nullptr;
    }

    mainWindow = new MainWindow;

    QVERIFY2( mainWindow, "useMenuExportKMLfile: mainWindow tests false");


    mainWindow->show();
    QTest::qWait(1000);

    // Enables focus and widget events
    QApplication::setActiveWindow( mainWindow );
    // Shortcuts won't work unless the window is active
    QVERIFY2( QTest::qWaitForWindowActive( mainWindow ), "mainWindow is not active" );

    // ---- Using shortkeys

    // Keyboard to Object Inventory menu
    QTest::keyClick( mainWindow, 'j', Qt::AltModifier );

    mainWindow->show();
    QTest::qWait(1000);



    QMenuBar *menuBar = mainWindow->findChild< QMenuBar * >( "menuBar" );
    QVERIFY2( menuBar, "useMenuImportToLoadSidescanFile: menuBar tests false");

    QMenu *menuObject_Inventory = menuBar->findChild< QMenu * >( "menuObject_Inventory" );
    QVERIFY2( menuObject_Inventory, "useMenuImportToLoadSidescanFile: menuObject_Inventory tests false");


    // Keyboard for Export Inventory
    QTest::keyClick( menuObject_Inventory, 'x', Qt::AltModifier );

    mainWindow->show();
    QTest::qWait(1000);



    QMenu *menuExport_Inventory = menuObject_Inventory->findChild< QMenu * >( "menuExport_Inventory" );
    QVERIFY2( menuExport_Inventory, "useMenuImportToLoadSidescanFile: menuExport_Inventory tests false");



    // Time out timer in case there is a failure while interacting with the modal window
    timerTimeOut->start( 5 * 1000 );

    QTimer::singleShot(500, this, SLOT(interactWithModalWindowToExportKMLfile() ) );

    // Keyboard for Export KML File
    QTest::keyClick( menuExport_Inventory, 'k', Qt::AltModifier );


}

void testGUI::interactWithModalWindowToExportKMLfile()
{

    qDebug() << tr( "Beginning of interactWithModalWindowToExportKMLfile()" );

    mainWindow->show();
    QTest::qWait(500);

    QWidget * modalWidget = QApplication::activeModalWidget();
    QVERIFY2( modalWidget, "interactWithModalWindowToExportKMLfile: modalWidget tests false");

    QVERIFY2( modalWidget->windowTitle() == tr( "Export as KML File" ),
              "interactWithModalWindowToExportKMLfile: modalWidget->windowTitle() is not 'Export as KML File'" );


    // Find the button to Save and to Cancel the modal window

    // The buttons are within a QDialogButtonBox

    QDialogButtonBox *buttonBox = modalWidget->findChild<QDialogButtonBox*>("buttonBox");
    QVERIFY2( buttonBox, "interactWithModalWindowToExportKMLfile: buttonBox tests false");


    // The buttons don't have object names,
    // I have to go through the list of buttons and find the button with
    // the desired text

    QList<QAbstractButton *> listButtonBox = buttonBox->buttons();

    QString cancelButtonTextWithAmpersand = tr( "&Cancel" );
	QString cancelButtonText = tr( "Cancel" );	
    QPushButton * cancelButton = nullptr;

    QString saveButtonText = tr( "&Save" );
    QPushButton * saveButton = nullptr;

    for (QAbstractButton *button : listButtonBox) {

        if ( button->text() == cancelButtonText || button->text() == cancelButtonTextWithAmpersand )
            cancelButton = static_cast<QPushButton * >( button );
        else if ( button->text() == saveButtonText )
            saveButton = static_cast<QPushButton * >( button );
    }

//    QVERIFY2( false, "interactWithModalWindowToExportKMLfile: false on purpose");

    QVERIFY2( cancelButton, "interactWithModalWindowToExportKMLfile: cancelButton tests false");
    QVERIFY2( cancelButton->isEnabled(), "interactWithModalWindowToExportKMLfile: cancelButton is not enabled");

    QVERIFY2( saveButton, "interactWithModalWindowToExportKMLfile: saveButton tests false");
    QVERIFY2( saveButton->isEnabled() == false, "interactWithModalWindowToExportKMLfile: saveButton is enabled while it should not if no filename is entered/selected");


    mainWindow->show();
    QTest::qWait(500);

    // Click cancel button to close the modal dialog without saving
    QTest::mouseClick(cancelButton, Qt::LeftButton);

    mainWindow->show();
    QTest::qWait(1000);


    interactWithModalWindowToExportKMLfileReachedTheEnd = true;

}



void testGUI::cleanAfterUseMenuExportKMLfile()
{
    std::cout << "\n\n\nBeginning of 'testGUI::cleanAfterUseMenuExportKMLfile()'" << std::endl;

    QVERIFY2( interactWithModalWindowToExportKMLfileReachedTheEnd, "cleanAfterUseMenuExportKMLfile: interactWithModalWindowToExportKMLfileReachedTheEnd is false" );


    if ( mainWindow ) {
        delete mainWindow;
        mainWindow = nullptr;
    }
}



#endif


void testGUI::useToolBarActionImportToLoadSidescanFileForFindingObjects()
{
    qDebug() << tr( "Beginning of 'useToolBarActionImportToLoadSidescanFileForFindingObjects()'" );

    // setup for the test
    interactWithModalWindowActionImportReachedTheEnd = false;

    if ( mainWindow ) {
        delete mainWindow;
        mainWindow = nullptr;
    }

    mainWindow = new MainWindow;

    QVERIFY2( mainWindow, "useToolBarActionImportToLoadSidescanFileForFindingObjects: mainWindow tests false");


    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->availableGeometry();

    float factorScreenSize = 0.7f;

    mainWindow->resize( static_cast<int>( floor( screenGeometry.width() * factorScreenSize ) ),
                  static_cast<int>( floor( screenGeometry.height() * factorScreenSize) ) );


//    mainWindow->showMaximized();



    // Get action for importing a sidescan file

    QAction * actionImport = mainWindow->findChild< QAction * >( "actionImport" );
    QVERIFY2( actionImport, "useToolBarActionImportToLoadSidescanFileForFindingObjects: actionImport tests false");


    mainWindow->show();
    QTest::qWait(1200);



    QToolBar * mainToolBar = mainWindow->findChild< QToolBar * >( "mainToolBar" );
    QVERIFY2( mainToolBar, "useToolBarActionImportToLoadSidescanFileForFindingObjects: mainToolBar tests false");


    QWidget *widgetForActionImport = mainToolBar->widgetForAction( actionImport );
    QVERIFY2( widgetForActionImport, "useToolBarActionImportToLoadSidescanFileForFindingObjects: widgetForActionImport tests false");


    // Time out timer in case there is a failure while interacting with the modal window
    timerTimeOut->start( 50 * 1000 ); // Time large enough to include the time it takes to load the files

    // Single shot timer for function that will interact with the modal window
    QTimer::singleShot(500, this, SLOT(interactWithModalWindowActionImport() ) );

    // Click the button to open the modal dialog
    QTest::mouseClick(widgetForActionImport, Qt::LeftButton);
}


void testGUI::operateMouseToCreateObjects()
{


    timerTimeOut->stop();

    std::cout << "\n" << std::endl;

    qDebug() << tr( "Beginning of 'testGUI::verifyResultOfImportToLoadSidescanFile'" );


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
                    "operateMouseToCreateObjects: interactWithModalWindowActionImportReachedTheEnd is false");
    }


    QVERIFY2( mainWindow, "operateMouseToCreateObjects: mainWindow tests false");



    QVERIFY2( mainWindow->projectWindow->model->getNbFiles() ==  2,
                qPrintable( "operateMouseToCreateObjects: the number of files in the projectWindow is "
                + QString::number( mainWindow->projectWindow->model->getNbFiles() )
                + " instead of 2") );



    // Files, tabs, and coordinates where to create objects

    std::string filenames[] = { "plane1.xtf", "scotsman3.xtf" };

    const int nbCols = 6;
    const int nbObjects = 4;

    int dataForSelection[ nbObjects ][ nbCols ] = {
        //  fileToSelect    tabIndex,   topLeftCornerX,     topLeftCornerY,     bottomRightCornerX,     bottomRightCornerY
        {   0,              1,          40,                 60,                 90,                     100 },
        {   0,              1,          20,                 120,                60,                     150 },
        {   0,              0,          20,                 120,                60,                     150 },
        {   1,              1,          40,                 60,                 90,                     100 },
        };


    for ( int count = 0; count < nbObjects; count++ ) {

        interactWithModalWindowObjectInformationReachedTheEnd = false;

        int * row = dataForSelection[ count ];

        useMouseToFormObjectBound( row[ 0 ], filenames[ row[ 0 ] ],
                                    row[ 1 ],
                                   row[ 2 ], row[ 3 ],
                                   row[ 4 ], row[ 5 ] );

        mainWindow->show();
        QTest::qWait(3000); // This time must be less than the timeout's time set inside useMouseToFormObjectBound()

        timerTimeOut->stop();

        std::cout << "\nAfter timerTimeOut->stop()\n"
                  << "interactWithModalWindowObjectInformationReachedTheEnd:" << std::boolalpha
                  <<  interactWithModalWindowObjectInformationReachedTheEnd << std::noboolalpha << std::endl;


        QVERIFY2( interactWithModalWindowObjectInformationReachedTheEnd,
                  qPrintable( "testGUI::operateMouseToCreateObjects: interactWithModalWindowObjectInformationReachedTheEnd is false for count="
                              + QString::number( count ) ) );

//        mainWindow->show();
//        QTest::qWait(3000);

    }


    // Verify that the inventory contains the correct number of objects

    // inventoryWindow (which uses QTableWidget class)

    std::cout << "\n\nmainWindow->inventoryWindow->inventoryTable->rowCount(): " << mainWindow->inventoryWindow->inventoryTable->rowCount()
              << "\nmainWindow->inventoryWindow->inventoryTable->columnCount(): " << mainWindow->inventoryWindow->inventoryTable->columnCount()<< std::endl;

    for ( int i=0; i< mainWindow->inventoryWindow->inventoryTable->rowCount(); i++ ) {
        std::cout << "count: " << i << ", \""
                  << mainWindow->inventoryWindow->inventoryTable->item(i, 0)->text().toStdString()
                  << "\": \"" << mainWindow->inventoryWindow->inventoryTable->item(i, 1)->text().toStdString()
                  << "\"\n";
    }



    QVERIFY2( mainWindow->inventoryWindow->inventoryTable->rowCount() ==  nbObjects,
                qPrintable( "operateMouseToCreateObjects: the number of objects in inventoryWindow is "
                + QString::number( mainWindow->inventoryWindow->inventoryTable->rowCount() )
                + " instead of the expected " + QString::number( nbObjects ) ) );




    // Save object images




    std::cout << "\nEnd of testGUI::operateMouseToCreateObjects()\n" << std::endl;


}

void testGUI::useMouseToFormObjectBound( int fileToSelect, std::string & filename, int tabIndex,
                                    int topLeftCornerX, int topLeftCornerY,
                                    int bottomRightCornerX, int bottomRightCornerY )
{

    std::cout << "\nBeginning of testGUI::useMouseToFormObjectBound()\n" << std::endl;

    // Select the file to be sure it is displayed

    QModelIndex indexFileToSelect = mainWindow->projectWindow->model->getModelIndexFileIndex( fileToSelect );

    // Scroll until it is visible
    mainWindow->projectWindow->tree->scrollTo( indexFileToSelect );

    QRect rectFileToSelect = mainWindow->projectWindow->tree->visualRect( indexFileToSelect );

    // Verify that the rectangle position corresponds to the same index in the model
    QModelIndex indexForPosition = mainWindow->projectWindow->tree->indexAt(
                                    rectFileToSelect.center() );

    QVERIFY2( indexFileToSelect == indexForPosition,
              qPrintable( "testGUI::useMouseToFormObjectBound: fileToSelect " + QString::number( fileToSelect )
                          + ":, indexFileToSelect is different from indexForPosition" ) );



    // Select the file
    QTest::mouseClick(mainWindow->projectWindow->tree->viewport(), Qt::LeftButton,
                      Qt::NoModifier,
                      rectFileToSelect.center() );


    QModelIndex currentIndex = mainWindow->projectWindow->tree->currentIndex();
    QVERIFY2( currentIndex.row() == fileToSelect,
              qPrintable( "testGUI::useMouseToFormObjectBound: fileToSelect " + QString::number( fileToSelect )
                          + ":, currentIndex.row() is different from fileToSelect" ) );

    std::string modelFilename = mainWindow->projectWindow->model->data(currentIndex, Qt::DisplayRole).toString().toStdString();

    QVERIFY2( modelFilename == filename,
              qPrintable( "testGUI::useMouseToFormObjectBound: filename for fileToSelect "
            + QString::number( fileToSelect ) + " has wrong text of '" +  tr( modelFilename.c_str() )
            + "' instead of the expected '" + tr( filename.c_str() ) + "'" ) );



    //    std::cout << "\n\ncurrentIndex.row(): " << currentIndex.row() << std::endl;

    // Give a bit of time to be sure the tabs are settled
    mainWindow->show();
    QTest::qWait(100);

    // Verify tabs
    QVERIFY2( mainWindow->tabs,
              qPrintable( "testGUI::useMouseToFormObjectBound: fileToSelect "
            + QString::number( fileToSelect ) + ", mainWindow->tabs tests false" ) );


    QTabBar *tabBar = mainWindow->tabs->tabBar();

    QVERIFY2( tabBar,
              qPrintable( "testGUI::useMouseToFormObjectBound: fileToSelect "
            + QString::number( fileToSelect ) + ", tabBar tests false" ) );


    // click on the tab

    const QPoint tabPos = tabBar->tabRect( tabIndex ).center();
    QTest::mouseClick( tabBar, Qt::LeftButton, {}, tabPos);

    mainWindow->show();
    QTest::qWait(500);

    QVERIFY2( mainWindow->tabs->currentIndex() == tabIndex,
              qPrintable( "testGUI::useMouseToFormObjectBound: fileToSelect "
            + QString::number( fileToSelect ) + ", the current tab index of "
            + QString::number( mainWindow->tabs->currentIndex() ) + " is different from the target of "
            + QString::number( tabIndex ) ) );


    // Get a handle on the label that displays the image

    QWidget * imageTab = mainWindow->tabs->currentWidget();

//    QSize size = imageTab->size();
//    std::cout << "\nimageTab: width: " << size.width()
//              << "\n          height: " << size.height() << "\n" << std::endl;

    QScrollArea * scrollArea = imageTab->findChild< QScrollArea * >( "scrollArea which holds the imageLabel" );

    QWidget * scrollAreaViewport = imageTab->findChild< QWidget * >( "qt_scrollarea_viewport" );

    ImageTabLabel * imageTablabel = scrollAreaViewport->findChild< ImageTabLabel * >( "imageLabel" );

//    // Get dimensions of the label, to know what maximum size an object could have
//    size = imageTablabel->size();
//    std::cout << "\nimageTablabel: width: " << size.width()
//              << "\n               height: " << size.height() << "\n" << std::endl;


    // Time out timer in case there is a failure while interacting with the modal window
    timerTimeOut->start( 5 * 1000 );

    QTimer::singleShot(500, this, SLOT(interactWithModalWindowObjectInformation() ) );

    // Press mouse's left button
    QTest::mousePress( imageTablabel, Qt::LeftButton, Qt::NoModifier, QPoint(topLeftCornerX,topLeftCornerY) );

    // Move mouse
    QTest::mouseMove( imageTablabel, QPoint(bottomRightCornerX,bottomRightCornerY), 20 ); // Necessary to put delay value in the function call even if there was a QTest::qWait(500) before the function call

    // Release mouse's left button
    QTest::mouseRelease( imageTablabel, Qt::LeftButton, Qt::NoModifier, QPoint(bottomRightCornerX,bottomRightCornerY), 20 ); // Necessary to put delay value in the function call even if there was a QTest::qWait(500) before the function call

}





void testGUI::interactWithModalWindowObjectInformation()
{
    qDebug() << tr( "Beginning of interactWithModalWindowObjectInformation()" );

    mainWindow->show();
    QTest::qWait(500);

    QWidget * modalWidget = QApplication::activeModalWidget();
    QVERIFY2( modalWidget, "interactWithModalWindowObjectInformation: modalWidget tests false");

    QVERIFY2( modalWidget->windowTitle() == tr( "Object Information" ),
              "interactWithModalWindowObjectInformation: modalWidget->windowTitle() is not 'Object Information'" );


    // Find the button to Accept and to Cancel the modal window

    // The buttons are within a QDialogButtonBox

    QDialogButtonBox *buttonBox = modalWidget->findChild<QDialogButtonBox*>("buttonBox");
    QVERIFY2( buttonBox, "interactWithModalWindowObjectInformation: buttonBox tests false");


    // The buttons don't have object names,
    // I have to go through the list of buttons and find the button with
    // the desired text

    QList<QAbstractButton *> listButtonBox = buttonBox->buttons();

    QString cancelButtonTextWithAmpersand = tr( "&Cancel" );
    QString cancelButtonText = tr( "Cancel" );
    QPushButton * cancelButton = nullptr;

    QString OKButtonTextWithAmpersand = tr( "&OK" );
    QString OKButtonText = tr( "OK" );
    QPushButton * buttonOK = nullptr;

    for (QAbstractButton *button : listButtonBox) {

        if ( button->text() == cancelButtonText || button->text() == cancelButtonTextWithAmpersand )
            cancelButton = static_cast<QPushButton * >( button );
        else if ( button->text() == OKButtonText || button->text() == OKButtonTextWithAmpersand )
            buttonOK = static_cast<QPushButton * >( button );
    }

//    QVERIFY2( false, "interactWithModalWindowObjectInformation: false on purpose");

    QVERIFY2( cancelButton, "interactWithModalWindowObjectInformation: cancelButton tests false");
    QVERIFY2( cancelButton->isEnabled(), "interactWithModalWindowObjectInformation: cancelButton is not enabled");

    QVERIFY2( buttonOK, "interactWithModalWindowObjectInformation: buttonOK tests false");
    QVERIFY2( buttonOK->isEnabled(), "interactWithModalWindowObjectInformation: buttonOK is not enabled");


    mainWindow->show();
    QTest::qWait(100);

    // Click OK button to keep the object
    QTest::mouseClick(buttonOK, Qt::LeftButton);

    mainWindow->show();
    QTest::qWait(100);

    interactWithModalWindowObjectInformationReachedTheEnd = true;
}



void testGUI::cleanAfterFindingObjects()
{
    std::cout << "\n\n\nBeginning of 'testGUI::cleanAfterFindingObjects()'" << std::endl;



    if ( mainWindow ) {
        delete mainWindow;
        mainWindow = nullptr;
    }
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
