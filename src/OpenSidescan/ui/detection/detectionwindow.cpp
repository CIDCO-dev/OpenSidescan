#include "detectionwindow.h"

#include <QProgressDialog>
#include <QCoreApplication>

#include <QStyledItemDelegate>


#include <QThread>

#include "workerdetection.h"



DetectionWindow::DetectionWindow(std::vector<SidescanFile *> & files,
                                 int & fastThresholdValue,
                                 int & fastTypeValue,
                                 bool & fastNonMaxSuppressionValue,
                                 int & dbscanEpsilonValue,
                                 int & dbscanMinPointsValue,
                                 int & mserDeltaValue,
                                 int & mserMinimumAreaValue,
                                 int & mserMaximumAreaValue,
                                 bool & mergeOverlappingBoundingBoxesValue,
                                 QWidget * parent):
                                files(files),
                                QDialog(parent),
                                fastThresholdValue(fastThresholdValue),
                                fastTypeValue(fastTypeValue),
                                fastNonMaxSuppressionValue(fastNonMaxSuppressionValue),
                                dbscanEpsilonValue(dbscanEpsilonValue),
                                dbscanMinPointsValue(dbscanMinPointsValue),
                                mserDeltaValue(mserDeltaValue),
                                mserMinimumAreaValue(mserMinimumAreaValue),
                                mserMaximumAreaValue(mserMaximumAreaValue),
                                mergeOverlappingBoundingBoxesValue(mergeOverlappingBoundingBoxesValue)

{
    initUI();
}

void DetectionWindow::initUI(){
    this->setWindowTitle("Find Objects");

    QVBoxLayout *mainLayout = new QVBoxLayout();

    cmbDetector = new QComboBox(this);
    connect(cmbDetector,SIGNAL(currentIndexChanged(int)),this,SLOT(detectorChanged(int)));

    //QStyledItemDelegate * delegate = new QStyledItemDelegate();
    //cmbDetector->setItemDelegate( delegate );



    mainLayout->addWidget(cmbDetector);

    mainLayout->addWidget(createDisplayParameterBox());

    //advanced parameters box
    advancedParameters = new QGroupBox(tr("Advanced Parameters"));

    QFormLayout * advancedParametersLayout = new QFormLayout();
    advancedParameters->setLayout(advancedParametersLayout);

    createFastParameterBox(advancedParametersLayout);
    createMserParameterBox(advancedParametersLayout);
    createDbscanParameterBox(advancedParametersLayout);

    mainLayout->addWidget(advancedParameters);

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    buttonBox->setObjectName( "buttonBox" );

    connect(buttonBox, &QDialogButtonBox::accepted, this, &DetectionWindow::ok);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &DetectionWindow::cancel);

    mainLayout->addWidget(buttonBox);

    this->setLayout(mainLayout);

    //Add detector types
    //If we added them before, the app would trigger a selected() event upon insertion and trigger the redraw login on uninitialized elements
    cmbDetector->addItem(tr("Large Objects (Ex: Shipwrecks)"),"shipwrecks");
    cmbDetector->addItem(tr("Machine Vision Mode (Expert Users Only)"),"machinevision");
}

void DetectionWindow::detectorChanged(int i){
    currentDetectorIndex = i;

    const char * id = cmbDetector->itemData(i).toString().toStdString().c_str();

    if(strcmp(id,"shipwrecks")==0){
        advancedParameters->setVisible(false);
    }
    if(strcmp(id,"machinevision")==0){
       advancedParameters->setVisible(true);
    }

    this->adjustSize();
}

void DetectionWindow::createFastParameterBox(QFormLayout * advancedParametersLayout){
    fastThreshold = new QLineEdit();
    fastThreshold->setValidator( new QIntValidator(1, 1000000, this) );
    fastThreshold->setAlignment(Qt::AlignRight);
    fastThreshold->setText(QString::fromStdString(std::to_string(fastThresholdValue)));

    advancedParametersLayout->addRow(new QLabel(tr("FAST Corner Intensity Threshold")),fastThreshold);

    fastType = new QComboBox(this);

    // Issue with combobox and dark style sheet
    // QComboBox expands the last chosen item
    // https://github.com/ColinDuquesnoy/QDarkStyleSheet/issues/200
    // Somewhat of a fix provided in the comments
    QStyledItemDelegate * delegate = new QStyledItemDelegate();
    fastType->setItemDelegate( delegate );

    //fastType->lineEdit()->setAlignment(Qt::AlignRight);
    fastType->addItem(tr("5-8"),cv::FastFeatureDetector::TYPE_5_8);
    fastType->addItem(tr("7-12"),cv::FastFeatureDetector::TYPE_7_12);
    fastType->addItem(tr("9-16"),cv::FastFeatureDetector::TYPE_9_16);
    fastType->setEditable(false);
    fastType->setCurrentIndex(fastType->findData(fastTypeValue));

    advancedParametersLayout->addRow(new QLabel(tr("FAST Corner Type")),fastType);

    fastNonMaxSuppression = new QCheckBox(this);
    advancedParametersLayout->addRow(new QLabel("FAST Use Non-maximal Suppression"),fastNonMaxSuppression);
}

void DetectionWindow::createDbscanParameterBox(QFormLayout * advancedParametersLayout){
    dbscanEpsilon = new QLineEdit();
    dbscanEpsilon->setValidator( new QIntValidator(1, 1000000, this) );
    dbscanEpsilon->setAlignment(Qt::AlignRight);
    dbscanEpsilon->setText(QString::fromStdString(std::to_string(dbscanEpsilonValue)));

    advancedParametersLayout->addRow(new QLabel(tr("DBSCAN Epsilon")),dbscanEpsilon);

    dbscanMinimumPointCount = new QLineEdit();
    dbscanMinimumPointCount->setValidator( new QIntValidator(1, 1000000, this) );
    dbscanMinimumPointCount->setAlignment(Qt::AlignRight);
    dbscanMinimumPointCount->setText(QString::fromStdString(std::to_string(dbscanMinPointsValue)));

    advancedParametersLayout->addRow(new QLabel(tr("DBSCAN Minimum Point Count")),dbscanMinimumPointCount);
}


void DetectionWindow::createMserParameterBox(QFormLayout * advancedParametersLayout){
    mserDelta = new QLineEdit();
    mserDelta->setValidator( new QIntValidator(1, 1000000, this) );
    mserDelta->setAlignment(Qt::AlignRight);
    mserDelta->setText(QString::fromStdString(std::to_string(mserDeltaValue)));

    advancedParametersLayout->addRow(new QLabel(tr("MSER Delta")),mserDelta);

    mserMinimumArea = new QLineEdit();
    mserMinimumArea->setValidator( new QIntValidator(1, 1000000, this) );
    mserMinimumArea->setAlignment(Qt::AlignRight);
    mserMinimumArea->setText(QString::fromStdString(std::to_string(mserMinimumAreaValue)));

    advancedParametersLayout->addRow(new QLabel(tr("MSER Minimum Area")),mserMinimumArea);

    mserMaximumArea = new QLineEdit();
    mserMaximumArea->setValidator( new QIntValidator(1, 1000000, this) );
    mserMaximumArea->setAlignment(Qt::AlignRight);
    mserMaximumArea->setText(QString::fromStdString(std::to_string(mserMaximumAreaValue)));

    advancedParametersLayout->addRow(new QLabel(tr("MSER Maximum Area")),mserMaximumArea);

}

QGroupBox * DetectionWindow::createDisplayParameterBox(){
    displayParameters = new QGroupBox(tr("Display"));

    QVBoxLayout * displayParameterLayout = new QVBoxLayout();
    displayParameters->setLayout(displayParameterLayout);

    mergeBoundingBoxes = new QCheckBox(tr("Merge overlapping bounding boxes"),this);
    mergeBoundingBoxes->setChecked(mergeOverlappingBoundingBoxesValue);
    displayParameterLayout->addWidget(mergeBoundingBoxes);


    return displayParameters;
}


void DetectionWindow::ok(){
    const char * id = cmbDetector->itemData(currentDetectorIndex).toString().toStdString().c_str();

    if(strcmp(id,"shipwrecks")==0){
        buildShipwreckDetector();
    }
    if(strcmp(id,"machinevision")==0){
        buildAdvancedDetector();
    }

    this->accept();
}

void DetectionWindow::buildShipwreckDetector(){
    Detector * detector = new RoiDetector(
                    300,
                    cv::FastFeatureDetector::TYPE_9_16,
                    false,
                    50,
                    20,
                    6,
                    320,
                    15000,
                    true
                );

    launchDetectionWorker(detector);
}

void DetectionWindow::buildAdvancedDetector(){
    std::string sFastThreshold = fastThreshold->text().toStdString();

    if(!sFastThreshold.empty()){
        fastThresholdValue = std::atoi(sFastThreshold.c_str());

        if(fastThresholdValue > 0){
                fastTypeValue = fastType->currentData().toInt();

                if(fastTypeValue > 0){

                    std::string sDbscanEpsilon = dbscanEpsilon->text().toStdString();

                    if(!sDbscanEpsilon.empty()){
                        dbscanEpsilonValue = std::atoi(sDbscanEpsilon.c_str());

                        if(dbscanEpsilonValue > 0){
                            std::string sDbscanMinPoints = dbscanMinimumPointCount->text().toStdString();

                            if(!sDbscanMinPoints.empty()){
                                dbscanMinPointsValue = std::atoi(sDbscanMinPoints.c_str());

                                if(dbscanMinPointsValue > 0){
                                    std::string sMserDelta = mserDelta->text().toStdString();

                                    if(!sMserDelta.empty()){
                                        mserDeltaValue = std::atoi(sMserDelta.c_str());

                                        if(mserDeltaValue > 0){
                                            std::string sMserMinimumArea = mserMinimumArea->text().toStdString();

                                            if(!sMserMinimumArea.empty()){
                                                mserMinimumAreaValue = std::atoi(sMserMinimumArea.c_str());

                                                std::string sMserMaximumArea = mserMaximumArea->text().toStdString();

                                                if(!sMserMaximumArea.empty()){
                                                    mserMaximumAreaValue = std::atoi(sMserMaximumArea.c_str());

                                                    fastNonMaxSuppressionValue = fastNonMaxSuppression->isChecked();
                                                    mergeOverlappingBoundingBoxesValue = mergeBoundingBoxes->isChecked();

                                                    Detector * detector = new RoiDetector(
                                                                    fastThresholdValue,
                                                                    fastTypeValue,
                                                                    fastNonMaxSuppressionValue,
                                                                    dbscanEpsilonValue,
                                                                    dbscanMinPointsValue,
                                                                    mserDeltaValue,
                                                                    mserMinimumAreaValue,
                                                                    mserMaximumAreaValue,
                                                                    mergeOverlappingBoundingBoxesValue
                                                                );

                                                    launchDetectionWorker(detector);
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    else{
                        //TODO: whine about missing DBSCAN Epsilon
                    }
                }
                else{
                    //TODO: whine about bad FAST Type
                }
        }
        else{
            //TODO: message box whining about bad threshold
        }
    }
    else{
        //TODO: whine about missing threshold
    }
}

void DetectionWindow::launchDetectionWorker(Detector * detector){
    QProgressDialog progress("Finding objects...", QString(), 0, files.size(), this);

    progress.setWindowModality(Qt::WindowModal);
    progress.setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint | Qt::WindowTitleHint);

    progress.setValue(0);
    progress.setMinimumDuration( 0 );


    QThread * workerThread = new QThread( this );

    WorkerDetection * worker = new WorkerDetection( *this , *detector);

    worker->moveToThread(workerThread);

    connect( workerThread, &QThread::finished, worker, &WorkerDetection::deleteLater );
    connect( workerThread, &QThread::started, worker, &WorkerDetection::doWork );

    connect( worker, &WorkerDetection::progress, &progress, &QProgressDialog::setValue);

    workerThread->start();

    progress.exec();

    workerThread->quit();
    workerThread->wait();

    delete detector;
}

void DetectionWindow::cancel(){
    this->reject();
}
