#include "detectionwindow.h"

#include <QProgressDialog>
#include <QCoreApplication>

DetectionWindow::DetectionWindow(std::vector<SidescanFile *> & files,
                                 int & fastThresholdValue,
                                 int & fastTypeValue,
                                 bool & fastNonMaxSuppressionValue,
                                 int & dbscanEpsilonValue,
                                 int & dbscanMinPointsValue,
                                 int & mserDeltaValue,
                                 int & mserMinimumAreaValue,
                                 int & mserMaximumAreaValue,
                                 bool & showFeatureMarkersValue,
                                 bool & mergeOverlappingBoundingBoxesValue,
                                 QWidget * parent):
                                files(files),
                                objects(objects),
                                QDialog(parent),
                                fastThresholdValue(fastThresholdValue),
                                fastTypeValue(fastTypeValue),
                                fastNonMaxSuppressionValue(fastNonMaxSuppressionValue),
                                dbscanEpsilonValue(dbscanEpsilonValue),
                                dbscanMinPointsValue(dbscanMinPointsValue),
                                mserDeltaValue(mserDeltaValue),
                                mserMinimumAreaValue(mserMinimumAreaValue),
                                mserMaximumAreaValue(mserMaximumAreaValue),
                                showFeatureMarkersValue(showFeatureMarkersValue),
                                mergeOverlappingBoundingBoxesValue(mergeOverlappingBoundingBoxesValue)

{
    initUI();
}

void DetectionWindow::initUI(){
    this->setWindowTitle("Find Objects");

    QVBoxLayout *mainLayout = new QVBoxLayout();

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

    connect(buttonBox, &QDialogButtonBox::accepted, this, &DetectionWindow::ok);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &DetectionWindow::cancel);

    mainLayout->addWidget(buttonBox);

    this->setLayout(mainLayout);
}

void DetectionWindow::createFastParameterBox(QFormLayout * advancedParametersLayout){
    fastThreshold = new QLineEdit();
    fastThreshold->setValidator( new QIntValidator(1, 1000000, this) );
    fastThreshold->setAlignment(Qt::AlignRight);
    fastThreshold->setText(QString::fromStdString(std::to_string(fastThresholdValue)));

    advancedParametersLayout->addRow(new QLabel(tr("FAST Corner Intensity Threshold")),fastThreshold);

    fastType = new QComboBox(this);
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

    showFeatureMarkers = new QCheckBox(tr("Show microfeature markers"),this);
    showFeatureMarkers->setChecked(showFeatureMarkersValue);
    displayParameterLayout->addWidget(showFeatureMarkers);


    mergeBoundingBoxes = new QCheckBox(tr("Merge overlapping bounding boxes"),this);
    mergeBoundingBoxes->setChecked(mergeOverlappingBoundingBoxesValue);
    displayParameterLayout->addWidget(mergeBoundingBoxes);


    return displayParameters;
}

void DetectionWindow::ok(){
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

                                                    QProgressDialog progress("Finding objects...", QString(), 0, files.size(), this);
                                                    progress.setWindowModality(Qt::ApplicationModal);
                                                    progress.setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint | Qt::WindowTitleHint);
                                                    progress.show();
                                                    QCoreApplication::processEvents();

                                                    int fileIdx = 0;
                                                    for(auto i=files.begin();i!=files.end();i++){
                                                        //for every file, every image

                                                        for(auto j=(*i)->getImages().begin();j != (*i)->getImages().end();j++){
                                                            OpencvHelper::detectObjects(
                                                                        (*j)->getObjects(),
                                                                        **i,
                                                                        **j,
                                                                        fastThresholdValue,
                                                                        fastTypeValue,
                                                                        fastNonMaxSuppressionValue = fastNonMaxSuppression->isChecked(),
                                                                        dbscanEpsilonValue,
                                                                        dbscanMinPointsValue,
                                                                        mserDeltaValue,
                                                                        mserMinimumAreaValue,
                                                                        mserMaximumAreaValue,
                                                                        showFeatureMarkersValue = showFeatureMarkers->isChecked(),
                                                                        mergeOverlappingBoundingBoxesValue = mergeBoundingBoxes->isChecked()
                                                                    );
                                                        }

                                                        progress.setValue(fileIdx);
                                                        QCoreApplication::processEvents();
                                                        fileIdx++;

                                                    }

                                                    progress.reset();
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


    this->accept();
}

void DetectionWindow::cancel(){
    this->reject();
}
