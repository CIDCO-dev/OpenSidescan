#include "detectionwindow.h"

#include <QProgressDialog>
#include <QCoreApplication>

#include <QStyledItemDelegate>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QMessageBox>
#include <QThread>
#include <QFileDialog>

#include "workerdetection.h"



DetectionWindow::DetectionWindow(Project & project,
                                 int & fastThresholdValue,
                                 cv::FastFeatureDetector::DetectorType & fastTypeValue,
                                 bool & fastNonMaxSuppressionValue,
                                 int & dbscanEpsilonValue,
                                 int & dbscanMinPointsValue,
                                 int & mserDeltaValue,
                                 int & mserMinimumAreaValue,
                                 int & mserMaximumAreaValue,
                                 bool & mergeOverlappingBoundingBoxesValue,
                                 QWidget * parent):
                                project(project),
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
	this->imageCount = project.getImageCount();
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

    //yolo parameters box
    yoloParameters = new QGroupBox(tr("Custom YoloV5 parameters"));

    QGridLayout * yoloParametersLayout = new QGridLayout();
    yoloParameters->setLayout(yoloParametersLayout);

    createYoloParameterBox(yoloParametersLayout);

    mainLayout->addWidget(yoloParameters);

    //dialog param
    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    buttonBox->setObjectName( "buttonBox" );

    connect(buttonBox, &QDialogButtonBox::accepted, this, &DetectionWindow::ok);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &DetectionWindow::cancel);

    mainLayout->addWidget(buttonBox);

    this->setLayout(mainLayout);

    //Add detector types
    //If we added them before, the app would trigger a selected() event upon insertion and trigger the redraw login on uninitialized elements
    cmbDetector->addItem(tr("Large Objects (Ex: Shipwrecks)"),"shipwrecks");

    //Ghost gear model
    cmbDetector->addItem(tr("Ghost gear (Crab trap)"),"Ghostgear");
    //yolov5 model
    cmbDetector->addItem(tr("Custom yoloV5 Model"),"CustomYolo");
    //TODO: reactivate this once debugged
    //cmbDetector->addItem(tr("Circular objects"),"circles");


    cmbDetector->addItem(tr("Machine Vision Mode (Expert Users Only)"),"machinevision");
}

void DetectionWindow::detectorChanged(int i){
    currentDetectorIndex = i;


    QString id = cmbDetector->itemData(i).toString();
    advancedParameters->setVisible(false);
    yoloParameters->setVisible(false);

    if(id.compare("shipwrecks",Qt::CaseSensitive)==0){
        //No parameter UI for shipwreck detector
    }
    if(id.compare("machinevision",Qt::CaseSensitive)==0){
       advancedParameters->setVisible(true);
    }
    if(id.compare("circles",Qt::CaseSensitive)==0){
        //No parameter UI for circle detector
    }
    if(id.compare("Ghostgear",Qt::CaseSensitive)==0){
        //No parameter UI for crab trap
    }
    if(id.compare("CustomYolo",Qt::CaseSensitive)==0){
        yoloParameters->setVisible(true);
    }

    this->adjustSize();
}

void DetectionWindow::createYoloParameterBox(QGridLayout * yoloParametersLayout){
    scoresThreshold = new QLineEdit();
    nmsThreshold = new QLineEdit();
    confidenceThreshold = new QLineEdit();
    modelPath = new QLineEdit();
    modelDialogButton = new QPushButton("...", this);

    connect(modelDialogButton, &QPushButton::clicked, this, &DetectionWindow::selectModel);

    scoresThreshold->insert("0.5");
    nmsThreshold->insert("0.45");
    confidenceThreshold->insert("0.3");

    yoloParametersLayout->addWidget(new QLabel(tr("model")),0,0);
    yoloParametersLayout->addWidget(modelPath,0,1);
    yoloParametersLayout->addWidget(modelDialogButton,0,2);
    yoloParametersLayout->addWidget(new QLabel(tr("scores threshold")),1,0);
    yoloParametersLayout->addWidget(scoresThreshold,1,1);
    yoloParametersLayout->addWidget(new QLabel(tr("non max suppression threshold")),2,0);
    yoloParametersLayout->addWidget(nmsThreshold,2,1);
    yoloParametersLayout->addWidget(new QLabel(tr("confidence threshold")),3,0);
    yoloParametersLayout->addWidget(confidenceThreshold,3,1);

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
    //const char * id = cmbDetector->itemData(currentDetectorIndex).toString().toStdString().c_str();

    if(cmbDetector->itemData(currentDetectorIndex).toString().toStdString().compare("shipwrecks")==0){
        buildShipwreckDetector();
    }
    if(cmbDetector->itemData(currentDetectorIndex).toString().toStdString().compare("machinevision")==0){
        buildAdvancedDetector();
    }
    if(cmbDetector->itemData(currentDetectorIndex).toString().toStdString().compare("circles")==0){
        buildHoughDetector();
    }
    if(cmbDetector->itemData(currentDetectorIndex).toString().toStdString().compare("Ghostgear")==0){
       buildGhostGearDetector();
    }
    if(cmbDetector->itemData(currentDetectorIndex).toString().toStdString().compare("CustomYolo")==0){
      buildYolov5Detector();
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

void DetectionWindow::buildHoughDetector(){
    Detector * detector = new HoughDetector();

    launchDetectionWorker(detector);
}

void DetectionWindow::buildAdvancedDetector(){
    std::string sFastThreshold = fastThreshold->text().toStdString();

    if(!sFastThreshold.empty()){
        fastThresholdValue = std::atoi(sFastThreshold.c_str());

        if(fastThresholdValue > 0){
            int temp_fastTypeValue = fastType->currentData().toInt();
            fastTypeValue = RoiDetector::toCvFastType(temp_fastTypeValue);

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

void DetectionWindow::buildGhostGearDetector(){

    QFileInfo modelPathInfo(QCoreApplication::applicationDirPath() + "/../models/crabtrapV1.onnx"); //will only work if compile with cmake for linux, or if installed with windows installer , in other words while developping and testing via qtcreator import the model manually
    Detector * detector = new Yolov5Detector((modelPathInfo.absoluteFilePath()).toStdString());
    launchDetectionWorker(detector);
}

void DetectionWindow::buildYolov5Detector(){

    float scoresThresholdValue;
    float nmsThresholdValue;
    float confidenceThresholdValue;
    std::string modelFilePath;

    if(!modelPath->text().isEmpty()){
        try{
            scoresThresholdValue = std::stof(scoresThreshold->text().toStdString());
            nmsThresholdValue = std::stof(nmsThreshold->text().toStdString());
            confidenceThresholdValue = std::stof(confidenceThreshold->text().toStdString());
            modelFilePath = modelPath->text().toStdString();
        }
        catch(const std::exception &e){
            QString errorMessage = e.what();
            QMessageBox msgBox;
            msgBox.setText(errorMessage);
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.exec();
        }
    }
    else{
        QMessageBox msgBox;
        msgBox.setText(tr("Please select a model"));
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
    }
    QFileInfo fileInfo(QString::fromStdString(modelFilePath));

    if(scoresThresholdValue >0 && scoresThresholdValue <1 &&
       nmsThresholdValue > 0 && nmsThresholdValue < 1 &&
       confidenceThresholdValue >0 && confidenceThresholdValue<1){
            Detector * detector = new Yolov5Detector((fileInfo.absoluteFilePath()).toStdString(), scoresThresholdValue, nmsThresholdValue, confidenceThresholdValue);
            launchDetectionWorker(detector);
    }
    else{
        QMessageBox msgBox;
        msgBox.setText(tr("All thresholds must be between 0 and 1"));
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
    }
}

void DetectionWindow::selectModel(){
    QString fileName = QFileDialog::getOpenFileName( this,
                                                      tr("Select Model"),
                                                      "",
                                                      tr("yoloV5 model (*.onnx)"),
                                                        nullptr,
                                                        QFileDialog::DontUseNativeDialog );
    modelPath->insert(fileName);

}

void DetectionWindow::launchDetectionWorker(Detector * detector){
    QProgressDialog progress("Finding objects...", QString(), 0, imageCount, this);

    progress.setWindowModality(Qt::WindowModal);
    progress.setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint | Qt::WindowTitleHint);

    progress.setValue(0);
    progress.setMinimumDuration( 0 );


    QThread * workerThread = new QThread( this );

    WorkerDetection * worker = new WorkerDetection(project, *this , *detector, this->imageCount);

    worker->moveToThread(workerThread);

    //connect( workerThread, &QThread::finished, worker, &WorkerDetection::deleteLater );
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
