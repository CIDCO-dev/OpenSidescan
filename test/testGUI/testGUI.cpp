#include <iostream>


#include <QtTest>

#include <QObject>

#include <QWidget>

#include <QToolBar>

#include <QLineEdit>
#include <QPushButton>


#include <QDialogButtonBox>



#include <QDebug>

#include <QMetaObject>





#include "../../src/OpenSidescan/mainwindow.h"



// https://doc.qt.io/qt-5/qtest-overview.html

class testGUI : public QObject
{
    Q_OBJECT

    // https://www.qtcentre.org/threads/23541-Displaying-GUI-events-with-QtTest
    // If want to show the GUI
    void eventLoop(const int msec);


public slots:
    void InteractWithModalWindowActionImport();

    void InteractWithModalWindowAlreadyAnActiveProject();

    void InteractWithModalWindowToSelectProjectToOpen();

//    void InteractWithContextMenu();

//    void InteractWithModalWindowDialogPlatform();

    void timeOutOccured();

private slots:

    // The following private slots that are not treated as test functions.


    void initTestCase();        // initTestCase() will be called before the first test function is executed.

//    void initTestCase_data();   // initTestCase_data() will be called to create a global test data table.

    void cleanupTestCase();     // cleanupTestCase() will be called after the last test function was executed.

//    void init();                // init() will be called before each test function is executed.

//    void cleanup();             // cleanup() will be called after every test function.





    // Test functions

    void useToolBarActionImportToLoadSidescanFile();
    void verifyResultOfUseToolBarActionImportToLoadSidescanFile();

    void useToolBarActionOpenProject();
    void verifyResultOfUseToolBarActionOpenProject();

//    void afterContextMenu();

//    void finish();


private:


    MainWindow * mainWindow;

    bool InteractWithModalWindowActionImportReachedTheEnd;
    bool InteractWithModalWindowAlreadyAnActiveProjectReachedTheEnd;
    bool InteractWithModalWindowToSelectProjectToOpenReachedTheEnd;

    QTimer *timerTimeOut;

};

void testGUI::eventLoop(const int msec)
{
    QEventLoop loop;
    QTimer timer;
    QObject::connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
    timer.setSingleShot(true);
    timer.start(msec);
    loop.exec();
}


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

    InteractWithModalWindowActionImportReachedTheEnd = false;
    InteractWithModalWindowAlreadyAnActiveProjectReachedTheEnd = false;
    InteractWithModalWindowToSelectProjectToOpenReachedTheEnd = false;

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


void testGUI::useToolBarActionImportToLoadSidescanFile()
{
//    QSKIP( "Skip the first test" );

    qDebug() << tr( "Beginning of 'useToolBarActionImportToLoadSidescanFile()'" );


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
    eventLoop(1200);


    QToolBar * mainToolBar = mainWindow->findChild< QToolBar * >( "mainToolBar" );
    QVERIFY2( mainToolBar, "useToolBarActionImportToLoadSidescanFile: mainToolBar tests false");


    QWidget *widgetForActionImport = mainToolBar->widgetForAction( actionImport );
    QVERIFY2( widgetForActionImport, "useToolBarActionImportToLoadSidescanFile: widgetForActionImport tests false");


    // Time out timer in case there is a failure while interacting with the modal window
    timerTimeOut->start( 10 * 1000 );

    // Single shot timer for function that will interact with the modal window
    QTimer::singleShot(500, this, SLOT(InteractWithModalWindowActionImport() ) );

    // Click the button to open the modal dialog
    QTest::mouseClick(widgetForActionImport, Qt::LeftButton);

}


void testGUI::verifyResultOfUseToolBarActionImportToLoadSidescanFile()
{
//    QSKIP( "Skip the first test" );


    timerTimeOut->stop();

    std::cout << "\n" << std::endl;

    qDebug() << tr( "Beginning of 'testGUI::verifyResultOfUseToolBarActionImportToLoadSidescanFile'" );


    if ( InteractWithModalWindowActionImportReachedTheEnd == false )
    {
        // Give time for the window to be closed after the instruction to close is sent
        // in the function responding to the timer
        QTest::qWait( 200 );

        if ( mainWindow ) {
            delete mainWindow;
            mainWindow = nullptr;
        }

        QVERIFY2( InteractWithModalWindowActionImportReachedTheEnd,
                    "verifyResultOfUseToolBarActionImportToLoadSidescanFile: InteractWithModalWindowActionImportReachedTheEnd is false");
    }


    QVERIFY2( mainWindow, "verifyResultOfUseToolBarActionImportToLoadSidescanFile: mainWindow tests false");


    // There should be one file in the tree model
    QVERIFY2( mainWindow->projectWindow->model->getNbFiles() ==  1,
                "verifyResultOfUseToolBarActionImportToLoadSidescanFile: the number of files in the projectWindow is different from 1");


    // Select the file to be sure it is displayed

    int fileToSelect = 0;
    QModelIndex indexFileToSelect = mainWindow->projectWindow->model->getModelIndexFileIndex( fileToSelect );

    // Scroll until it is visible
    mainWindow->projectWindow->tree->scrollTo( indexFileToSelect );

    QRect rectFileToSelect = mainWindow->projectWindow->tree->visualRect( indexFileToSelect );

    // Verify that the rectangle position corresponds to the same index in the model
    QModelIndex indexForPosition = mainWindow->projectWindow->tree->indexAt(
                                    rectFileToSelect.center() );

    QVERIFY2( indexFileToSelect == indexForPosition,
                "verifyResultOfUseToolBarActionImportToLoadSidescanFile: indexFileToSelect is different from indexForPosition");

    // Select the file
    QTest::mouseClick(mainWindow->projectWindow->tree->viewport(), Qt::LeftButton,
                      Qt::NoModifier,
                      rectFileToSelect.center() );


    QModelIndex currentIndex = mainWindow->projectWindow->tree->currentIndex();
    QVERIFY2( currentIndex.row() == fileToSelect,
                "verifyResultOfUseToolBarActionImportToLoadSidescanFile: currentIndex.row() is different from fileToSelect");


//    std::cout << "\n\ncurrentIndex.row(): " << currentIndex.row() << std::endl;

    // Give a bit of time to be sure the tabs are settled
    mainWindow->show();
    eventLoop(100);

    // Verify tabs

    QVERIFY2( mainWindow->tabs,
                "verifyResultOfUseToolBarActionImportToLoadSidescanFile:  mainWindow->tabs tests false");

    QVERIFY2( mainWindow->tabs->count() == 3,
                "verifyResultOfUseToolBarActionImportToLoadSidescanFile: the number of tabs is different from 3");


    QVERIFY2( mainWindow->tabs->tabText( 0 ).toStdString() == "Channel 0",
                "verifyResultOfUseToolBarActionImportToLoadSidescanFile: Channel 0 has the wrong tabText");
    QVERIFY2( mainWindow->tabs->tabText( 1 ).toStdString() == "Channel 1",
                "verifyResultOfUseToolBarActionImportToLoadSidescanFile: Channel 0 has the wrong tabText");
    QVERIFY2( mainWindow->tabs->tabText( 2 ).toStdString() == "Channel 2",
                "verifyResultOfUseToolBarActionImportToLoadSidescanFile: Channel 0 has the wrong tabText");

    QVERIFY2( mainWindow->tabs->currentIndex() == 0,
                "verifyResultOfUseToolBarActionImportToLoadSidescanFile: The current tab index is different from zero");


    QTabBar *tabBar = mainWindow->tabs->tabBar();

    QVERIFY2( tabBar,
                "verifyResultOfUseToolBarActionImportToLoadSidescanFile: tabBar tests false");

    QVERIFY2( tabBar->count() == 3,
                "verifyResultOfUseToolBarActionImportToLoadSidescanFile: tabBar->count() is different from 3");


    // Click on second tab
    int index = 1;
    const QPoint tabPos = tabBar->tabRect( index ).center();
    QTest::mouseClick( tabBar, Qt::LeftButton, {}, tabPos);

    mainWindow->show();
    eventLoop(100);

    QVERIFY2( mainWindow->tabs->currentIndex() == index,
                "verifyResultOfUseToolBarActionImportToLoadSidescanFile: The current tab index is different from the target");


    // Verifying file properties (which uses QTableWidget class)

    // Verify the number of channels
    QString searchtext = "Channels (Sonar)";

    QList<QTableWidgetItem *> items = mainWindow->fileInfo->propertiesTable->findItems(searchtext, Qt::MatchExactly);

//    std::cout << "\n\nitems.size(): " << items.size() << "\n"
//              << "items[ 0 ]->row(): " << items[ 0 ]->row() << "\n"
//              << "items[ 0 ]->column(): " << items[ 0 ]->column() << "\n"
//              << "items[ 0 ]->data( Qt::DisplayRole ).toString(): " << items[ 0 ]->data( Qt::DisplayRole ).toString().toStdString() << "\n"
//              << "second column: " << mainWindow->fileInfo->propertiesTable->item( items[ 0 ]->row(), 1 )->data( Qt::DisplayRole ).toString().toStdString() << "\n"
//              << std::endl;

    QVERIFY2( items.size() == 1,
                "verifyResultOfUseToolBarActionImportToLoadSidescanFile: items.size() is different from 1");

    QVERIFY2( items[ 0 ]->column() == 0,
                "verifyResultOfUseToolBarActionImportToLoadSidescanFile: searchtext found in the wrong column");

    std::string nbChannelsString = mainWindow->fileInfo->propertiesTable->item( items[ 0 ]->row(), 1 )->data( Qt::DisplayRole ).toString().toStdString();
    QVERIFY2( nbChannelsString == "3",
                "verifyResultOfUseToolBarActionImportToLoadSidescanFile: wrong number of channels in propertiesTable");


    mainWindow->show();
    eventLoop(3000);

    if ( mainWindow ) {
        delete mainWindow;
        mainWindow = nullptr;
    }


}


void testGUI::useToolBarActionOpenProject()
{

//    QSKIP( "Skip the second test" );

    qDebug() << tr( "Beginning of 'useToolBarActionOpenProject()'" );

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

    // Time out timer in case there is a failure while interacting with the modal window
    timerTimeOut->start( 10 * 1000 );

    QTimer::singleShot(500, this, SLOT(InteractWithModalWindowAlreadyAnActiveProject() ) );

    // Click the button to open the modal dialog
    QTest::mouseClick(widgetForActionOpenProject, Qt::LeftButton);

}

void testGUI::verifyResultOfUseToolBarActionOpenProject()
{
//    QSKIP( "Skip the second test" );

    timerTimeOut->stop();

    std::cout << "\n" << std::endl;

    std::cout << "\n\nBeginning of testGUI::verifyResultOfUseToolBarActionOpenProject()\n" << std::endl;

    if ( InteractWithModalWindowToSelectProjectToOpenReachedTheEnd == false )
    {
        // Give time for the window to be closed after the instruction to close is sent
        // in the function responding to the timer
        QTest::qWait( 200 );

        if ( mainWindow ) {
            delete mainWindow;
            mainWindow = nullptr;
        }

        QVERIFY2( InteractWithModalWindowToSelectProjectToOpenReachedTheEnd,
                    "verifyResultOfUseToolBarActionOpenProject: InteractWithModalWindowActionImportReachedTheEnd is false");
    }

    QVERIFY2( mainWindow, "verifyResultOfUseToolBarActionOpenProject: mainWindow tests false");

    // There should be five files in the tree model
    QVERIFY2( mainWindow->projectWindow->model->getNbFiles() ==  5,
                "verifyResultOfUseToolBarActionOpenProject: the number of files in the projectWindow is different from 5");

    mainWindow->show();
    eventLoop(1000);


    // Verify the file names in the projectWindow
    // TODO:


    // Select a file

    int fileToSelect = 4;
    QModelIndex indexFileToSelect = mainWindow->projectWindow->model->getModelIndexFileIndex( fileToSelect );

    // Scroll until it is visible
    mainWindow->projectWindow->tree->scrollTo( indexFileToSelect );

    QRect rectFileToSelect = mainWindow->projectWindow->tree->visualRect( indexFileToSelect );

    // Verify that the rectangle position corresponds to the same index in the model
    QModelIndex indexForPosition = mainWindow->projectWindow->tree->indexAt(
                                    rectFileToSelect.center() );

    QVERIFY2( indexFileToSelect == indexForPosition,
                "verifyResultOfUseToolBarActionOpenProject: indexFileToSelect is different from indexForPosition");

    // Select the file
    QTest::mouseClick(mainWindow->projectWindow->tree->viewport(), Qt::LeftButton,
                      Qt::NoModifier,
                      rectFileToSelect.center() );

    // Give a bit of time to be sure the tabs are settled
    mainWindow->show();
    eventLoop(100);

    QModelIndex currentIndex = mainWindow->projectWindow->tree->currentIndex();
    QVERIFY2( currentIndex.row() == fileToSelect,
                "verifyResultOfUseToolBarActionOpenProject: currentIndex.row() is different from fileToSelect");

    std::cout << "File name: "
        << mainWindow->projectWindow->model->data(currentIndex, Qt::DisplayRole).toString().toStdString() << std::endl;


//    std::cout << "\n\ncurrentIndex.row(): " << currentIndex.row() << std::endl;

    // Give a bit of time to be sure the tabs are settled
    mainWindow->show();
    eventLoop(100);







//    QModelIndex currentIndex = mainWindow->projectWindow->tree->currentIndex();

//    std::cout << "\n\ncurrentIndex.row(): " << currentIndex.row() << std::endl;


////    // The following does not work to set the current index
////    mainWindow->projectWindow->tree->setCurrentIndex(
////                mainWindow->projectWindow->tree->model()->index( 2, 0 ) );

////    view.setCurrentIndex(view.model()->index(row, column));

//    // Click on an item in the treeview
////    QTest::mouseClick(mainWindow->projectWindow->tree->viewport(), Qt::LeftButton, Qt::NoModifier, QPoint( 60, 60) );

////    int xPos = mainWindow->projectWindow->tree->columnViewportPosition(0) + 2; // add 2 pixels
////    int yPos = mainWindow->projectWindow->tree-

//    // The following works
//    QTest::mouseClick(mainWindow->projectWindow->tree->viewport(), Qt::LeftButton, Qt::NoModifier, QPoint( 60, 60) );
////    QTest::mouseClick(mainWindow->projectWindow->tree->viewport(), Qt::LeftButton, Qt::NoModifier, QPoint( 60, 5) );

////    // Entire dimension of tree viewport, not just where they entries are.
////    QRect a = mainWindow->projectWindow->tree->viewport()->rect();


////    int height = a.bottom() - a.top();
////    int width = a.right() - a.left();


////    std::cout << "\n\na.top():    " << a.top() << "\n"
////                  << "a.bottom(): " << a.bottom() << "\n"
////                  << "a.left():   " << a.left() << "\n"
////                  << "a.right():  " << a.right() << "\n"
////                  << "height:     " << height << "\n"
////                  << "width:      " << width << "\n"
////                  << "\n" << std::endl;

////   int indexFileToClick = 1;
////   int verticalPos = static_cast< int > ( ( indexFileToClick + 1.5 ) * height /
////                                    ( mainWindow->projectWindow->model->getNbFiles() + 1 ) );
////   QTest::mouseClick(mainWindow->projectWindow->tree->viewport(),
////                     Qt::LeftButton, Qt::NoModifier,
////                     QPoint( width / 2, verticalPos) );







//    currentIndex = mainWindow->projectWindow->tree->currentIndex();
//    std::cout << "\n\ncurrentIndex.row(): " << currentIndex.row() << std::endl;


//    QModelIndexList listSelected = mainWindow->projectWindow->tree->selectionModel()->selectedIndexes();

//    std::cout << "\n\nlistSelected.size(): " << listSelected.size() << "\n" << std::endl;



//    if ( currentIndex.isValid() ) {
//        if ( mainWindow->projectWindow->model->isFilesNode( currentIndex ) == false )
//            std::cout << "File name: "
//                  << mainWindow->projectWindow->model->data(currentIndex, Qt::DisplayRole).toString().toStdString() << std::endl;
//        else
//            std::cout << "File node is the current index\n" << std::endl;
//    } else { // e.g. no selection
//        std::cout << "currentIndex is not valid\n" << std::endl;

//    }







//    std::cout << "\n\nmainWindow->projectWindow->model->rowCount(): "
//              << mainWindow->projectWindow->model->rowCount() << "\n" << std::endl;

//    std::cout << "\n\nmainWindow->projectWindow->model->getNbFiles(): "
//                 << mainWindow->projectWindow->model->getNbFiles() << "\n"
//              << std::endl;



//    mainWindow->show();
//    eventLoop(1000);


//    int fileToSelect = 4;
//    QModelIndex indexFileToSelect = mainWindow->projectWindow->model->getModelIndexFileIndex( fileToSelect );

//    // Scroll until it is visible
//    mainWindow->projectWindow->tree->scrollTo( indexFileToSelect );


//    // Set the index directly without mouse click
////    mainWindow->projectWindow->tree->setCurrentIndex( indexFileToSelect );

//    QRect rectFileToSelect = mainWindow->projectWindow->tree->visualRect( indexFileToSelect );


//    int height = rectFileToSelect.bottom() - rectFileToSelect.top();
//    int width = rectFileToSelect.right() - rectFileToSelect.left();


//    std::cout << "\n\nrectFileToSelect.top():    " << rectFileToSelect.top() << "\n"
//                  << "rectFileToSelect.bottom(): " << rectFileToSelect.bottom() << "\n"
//                  << "rectFileToSelect.left():   " << rectFileToSelect.left() << "\n"
//                  << "rectFileToSelect.right():  " << rectFileToSelect.right() << "\n"
//                  << "height:     " << height << "\n"
//                  << "width:      " << width << "\n" << std::endl;


//    int horizontalPosition = static_cast<int>( ( rectFileToSelect.right() + rectFileToSelect.left() ) / 2 );
//    int verticalPosition = static_cast<int>( ( rectFileToSelect.bottom() + rectFileToSelect.top() ) / 2 );

//    std::cout << "\n\nhorizontalPosition:            " << horizontalPosition << "\n"
//                  << "verticalPosition:              " << verticalPosition << "\n"
//                  << "rectFileToSelect.center().x(): " << rectFileToSelect.center().x() << "\n"
//                  << "rectFileToSelect.center().y(): " << rectFileToSelect.center().y() << "\n"
//                  << std::endl;

//    // Verify that the position corresponds to the same index
//    QModelIndex indexForPosition = mainWindow->projectWindow->tree->indexAt(
//                                    rectFileToSelect.center() );

//    if ( indexFileToSelect == indexForPosition )
//        std::cout << "\n\nSame Index\n" << std::endl;
//    else
//        std::cout << "\n\nDifferent Index\n" << std::endl;



//    QTest::mouseClick(mainWindow->projectWindow->tree->viewport(), Qt::LeftButton,
//                      Qt::NoModifier,
//                      rectFileToSelect.center() );


//    currentIndex = mainWindow->projectWindow->tree->currentIndex();
//    std::cout << "\n\ncurrentIndex.row(): " << currentIndex.row() << std::endl;


//    std::cout << "\n\nlistSelected.size(): " << listSelected.size() << "\n" << std::endl;



//    if ( currentIndex.isValid() ) {
//        if ( mainWindow->projectWindow->model->isFilesNode( currentIndex ) == false )
//            std::cout << "File name: "
//                  << mainWindow->projectWindow->model->data(currentIndex, Qt::DisplayRole).toString().toStdString() << std::endl;
//        else
//            std::cout << "File node is the current index\n" << std::endl;
//    } else { // e.g. no selection
//        std::cout << "currentIndex is not valid\n" << std::endl;

//    }

//    mainWindow->show();
//    eventLoop(1000);


//    // Trying to use Right click to remove a file from the project


////    QTimer::singleShot(500, this, SLOT(InteractWithContextMenu() ) );


//    // Does not work
//    QTest::mouseClick(mainWindow->projectWindow->tree->viewport(), Qt::RightButton,
//                      Qt::NoModifier,
//                      rectFileToSelect.center() );







//    // Does not work: selects the previous file and does trigger context menu
////    QTest::mouseMove(mainWindow->projectWindow->tree->viewport(),
////                      rectFileToSelect.center() );

////    QTest::mouseClick(mainWindow->projectWindow->tree->viewport(), Qt::RightButton );



    mainWindow->show();
    eventLoop(5000);




    if ( mainWindow )
    {
        delete mainWindow;
        mainWindow = nullptr;
    }


}


//void testGUI::afterContextMenu()
//{
//    qDebug() << tr( "Beginning of 'testGUI::afterContextMenu()'" );

//    mainWindow->show();
//    eventLoop(10000);

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
//    eventLoop(5000);

//}



void testGUI::InteractWithModalWindowActionImport()
{
    qDebug() << tr( "Beginning of InteractWithModalWindowActionImport()" );

    mainWindow->show();
    eventLoop(500);

    QWidget * modalWidget = QApplication::activeModalWidget();    
    QVERIFY2( modalWidget, "InteractWithModalWindowActionImport: modalWidget tests false");

    QVERIFY2( modalWidget->windowTitle() == tr( "Import Sidescan Files" ),
              "InteractWithModalWindowActionImport: modalWidget->windowTitle() is not 'Import Sidescan Files'" );


    QLineEdit * lineEdit = modalWidget->findChild<QLineEdit*>("fileNameEdit");
    QVERIFY2( lineEdit, "InteractWithModalWindowActionImport: lineEdit tests false");


    // Number of characters currently present in the QLineEdit
    int nbBackspaces = lineEdit->text().length();

    // Use backspaces to clear the current content
    for ( int count = 0; count < nbBackspaces; count++ )
        QTest::keyClick(lineEdit, Qt::Key_Backspace, Qt::NoModifier, 10 );


    mainWindow->show();
    eventLoop(100);

    // TODO: relative path
    QString filename = tr( "/home/chris/Worskpace/OpenSidescan/test/data/wrecks/plane1.xtf" );

    QTest::keyClicks(lineEdit, filename );

    QVERIFY2( lineEdit->text() == filename, "InteractWithModalWindowActionImport: filename is not the same in the QLineEdit");


    // Find the button to accept and close the modal window

    // The buttons are within a QDialogButtonBox

    QDialogButtonBox *buttonBox = modalWidget->findChild<QDialogButtonBox*>("buttonBox");
    QVERIFY2( buttonBox, "InteractWithModalWindowActionImport: buttonBox tests false");


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

    QVERIFY2( acceptButton, "InteractWithModalWindowActionImport: acceptButton tests false");
    QVERIFY2( acceptButton->isEnabled(), "InteractWithModalWindowActionImport: acceptButton is not enabled");



//    std::cout << "\n\n" << std::endl;


    mainWindow->show();
    eventLoop(500);

    // Click button to close the modal dialog
    QTest::mouseClick(acceptButton, Qt::LeftButton);

    mainWindow->show();
    eventLoop(500);


    InteractWithModalWindowActionImportReachedTheEnd = true;

}


void testGUI::InteractWithModalWindowAlreadyAnActiveProject()
{
    qDebug() << tr( "Beginning of InteractWithModalWindowAlreadyAnActiveProject()" );

    mainWindow->show();
    eventLoop(500);

    qDebug() << tr( "After starting AlreadyAnActiveProject modal window" );

//    QVERIFY2( false, "InteractWithModalWindowAlreadyAnActiveProject: false on purpose");


    QWidget * modalWidget = QApplication::activeModalWidget();
    QVERIFY2( modalWidget, "InteractWithModalWindowAlreadyAnActiveProject: modalWidget tests false");

//    std::cout << "\n\nmodalWidget: " << modalWidget << "\n" << std::endl;

//    qDebug() << tr( "modalWidget->objectName(): " ) << modalWidget->objectName();

//    qDebug() << tr( "modalWidget->windowTitle(): " ) << modalWidget->windowTitle();



    QDialogButtonBox *buttonBox = modalWidget->findChild<QDialogButtonBox*>("qt_msgbox_buttonbox");
    QVERIFY2( buttonBox, "InteractWithModalWindowAlreadyAnActiveProject: buttonBox tests false");

    QList<QAbstractButton *> listButtonBox = buttonBox->buttons();

    QString OKButtonText = tr( "&OK" );
    QPushButton * buttonOK = nullptr;

    for (QAbstractButton *button : listButtonBox) {
        if ( button->text() == OKButtonText )
            buttonOK = static_cast<QPushButton * >( button );
    }

    QVERIFY2( buttonOK, "InteractWithModalWindowAlreadyAnActiveProject: buttonOK tests false");

    // Time out timer in case there is a failure while interacting with the modal window
    timerTimeOut->start( 60 * 1000 ); // Time large enough to include the time it takes to load the files

    QTimer::singleShot(500, this, SLOT(InteractWithModalWindowToSelectProjectToOpen() ) );


    // Click the button to open the modal dialog
    QTest::mouseClick(buttonOK, Qt::LeftButton);

    InteractWithModalWindowAlreadyAnActiveProjectReachedTheEnd = true;


}


void testGUI::InteractWithModalWindowToSelectProjectToOpen()
{

    std::cout << "\n" << std::endl;

    qDebug() << tr( "Beginning of 'testGUI::InteractWithModalWindowToSelectProjectToOpen'" );


    if ( InteractWithModalWindowAlreadyAnActiveProjectReachedTheEnd == false )
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

        QVERIFY2( InteractWithModalWindowAlreadyAnActiveProjectReachedTheEnd,
                    "InteractWithModalWindowToSelectProjectToOpen: InteractWithModalWindowAlreadyAnActiveProjectReachedTheEnd is false");
    }

//    QVERIFY2( mainWindow, "InteractWithModalWindowToSelectProjectToOpen: mainWindow tests false");



    mainWindow->show();
    eventLoop(1200);

//    QVERIFY2( false, "InteractWithModalWindowToSelectProjectToOpen: false on purpose");


    QWidget * modalWidget = QApplication::activeModalWidget();
    QVERIFY2( modalWidget, "InteractWithModalWindowToSelectProjectToOpen: modalWidget tests false");

    QVERIFY2( modalWidget->windowTitle() == tr( "Sidescan Project Files" ),
              "InteractWithModalWindowToSelectProjectToOpen: modalWidget->windowTitle() is not 'Sidescan Project Files'" );


    QLineEdit *lineEdit = modalWidget->findChild<QLineEdit*>("fileNameEdit");
    QVERIFY2( lineEdit, "InteractWithModalWindowToSelectProjectToOpen: lineEdit tests false");


    // Number of characters currently present in the QLineEdit
    int nbBackspaces = lineEdit->text().length();

    // Use backspaces to clear the current content
    for ( int count = 0; count < nbBackspaces; count++ )
        QTest::keyClick(lineEdit, Qt::Key_Backspace, Qt::NoModifier, 10 );


    mainWindow->show();
    eventLoop(100);

    // TODO: relative path
    QString filename = tr( "/home/chris/Worskpace/OpenSidescan/test/testProject/TestProject5Files.ssp" );

    QTest::keyClicks(lineEdit, filename );

    QVERIFY2( lineEdit->text() == filename, "InteractWithModalWindowToSelectProjectToOpen: filename is not the same in the QLineEdit");


    // Find the button to accept and close the modal window

    // The buttons are within a QDialogButtonBox

    QDialogButtonBox *buttonBox = modalWidget->findChild<QDialogButtonBox*>("buttonBox");
    QVERIFY2( buttonBox, "InteractWithModalWindowToSelectProjectToOpen: buttonBox tests false");

    QList<QAbstractButton *> listButtonBox = buttonBox->buttons();

    QString acceptButtonText = tr( "&Open" );
    QPushButton * acceptButton = nullptr;

    for (QAbstractButton *button : listButtonBox) {

        if ( button->text() == acceptButtonText )
            acceptButton = static_cast<QPushButton * >( button );
    }

    QVERIFY2( acceptButton, "InteractWithModalWindowToSelectProjectToOpen: acceptButton tests false");
    QVERIFY2( acceptButton->isEnabled(), "InteractWithModalWindowToSelectProjectToOpen: acceptButton is not enabled");


    mainWindow->show();
    eventLoop(500);

    // Click button to close the modal dialog
    QTest::mouseClick(acceptButton, Qt::LeftButton);

    mainWindow->show();
    eventLoop(500);

    InteractWithModalWindowToSelectProjectToOpenReachedTheEnd = true;

}


QTEST_MAIN(testGUI)

#include "testGUI.moc"
