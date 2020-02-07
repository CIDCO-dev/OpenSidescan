
#include <QProgressDialog>
#include <QCoreApplication>

#include <QThread>

#include <QPushButton>

#include <QFormLayout>
#include <QGridLayout>
#include <QStringListModel>


#include "trainingsampleswindow.h"



TrainingSamplesWindow::TrainingSamplesWindow( const QString & folder,
                                 const ParameterscvCreateTrainingSamples & parameters )
    : folder( folder ), parameters( parameters ),
      userDidCancel( false )
{
    initUI();
}

void TrainingSamplesWindow::initUI(){
    this->setWindowTitle("Parameters to Create Positive Samples");








    /* Parameters to cvCreateTrainingSamples()

    -num <number_of_samples> : Number of positive samples to generate.

    -bgcolor <background_color> : Background color (currently grayscale images are assumed);
            the background color denotes the transparent color.
            Since there might be compression artifacts, the amount of color
            tolerance can be specified by -bgthresh. All pixels within
            bgcolor-bgthresh and bgcolor+bgthresh range are interpreted as transparent.
    -bgthresh <background_color_threshold>

    -inv : If specified, colors will be inverted.
    -randinv : If specified, colors will be inverted randomly.

    -maxidev <max_intensity_deviation> : Maximal intensity deviation of pixels in foreground samples

    -maxxangle <max_x_rotation_angle> : Maximal rotation angle towards x-axis, must be given in radians.
    -maxyangle <max_y_rotation_angle> : Maximal rotation angle towards y-axis, must be given in radians.
    -maxzangle <max_z_rotation_angle> : Maximal rotation angle towards z-axis, must be given in radians.

    -w <sample_width> : Width (in pixels) of the output samples.
    -h <sample_height> : Height (in pixels) of the output samples.


    */





    /*

    - Path where to put the file structure

    -num <number_of_samples> : Number of positive samples to generate.

    Group Colors and Intensity -------------------------------------------------

    -bgcolor <background_color> : Background color (currently grayscale images are assumed);
            the background color denotes the transparent color.
            Since there might be compression artifacts, the amount of color
            tolerance can be specified by -bgthresh. All pixels within
            bgcolor-bgthresh and bgcolor+bgthresh range are interpreted as transparent.
    -bgthresh <background_color_threshold>

    // Colors inversion: No, Yes, Random (combine the following information into a single widget)
        -inv : If specified, colors will be inverted.
        -randinv : If specified, colors will be inverted randomly.

    -maxidev <max_intensity_deviation> : Maximal intensity deviation of pixels in foreground samples


    Group Rotation (user enters values in degrees) ---------------------------------------------

    -maxxangle <max_x_rotation_angle> : Maximal rotation angle towards x-axis, must be given in radians.
    -maxyangle <max_y_rotation_angle> : Maximal rotation angle towards y-axis, must be given in radians.
    -maxzangle <max_z_rotation_angle> : Maximal rotation angle towards z-axis, must be given in radians.

    Group ---------------------------------------------

    SubGroup
    -w <sample_width> : Width (in pixels) of the output samples.


    SubGroup
    -h <sample_height> : Height (in pixels) of the output samples.


    */


    QVBoxLayout *mainLayout = new QVBoxLayout();



    // Layout for path
    QGridLayout *layoutPath= new QGridLayout;

    mainLayout->addLayout( layoutPath );


    QLabel * pathLabel = new QLabel( tr( "Path" ) );

    pathLineEdit = new QLineEdit;

    pathLabel->setBuddy( pathLineEdit );

    QPushButton *pathBrowseButton = new QPushButton( "Browse" );

    layoutPath->addWidget( pathLabel, 0, 0 );
    layoutPath->addWidget( pathLineEdit, 0, 1 );
    layoutPath->addWidget( pathBrowseButton, 0, 2 );


    // num
    QFormLayout * numLayout = new QFormLayout();

    mainLayout->addLayout( numLayout );

    numLineEdit = new QLineEdit();

    numLineEdit->setAlignment(Qt::AlignRight);
    numLineEdit->setText( QString::number( parameters.num ) );

    numLayout->addRow( new QLabel(tr("Number of positive samples to create per object")), numLineEdit);

    mainLayout->addWidget( createColorsAndIntensityBox() );






    setLayout(mainLayout);


//    mainLayout->addWidget(createDisplayParameterBox());

//    //advanced parameters box
//    advancedParameters = new QGroupBox(tr("Advanced Parameters"));

//    QFormLayout * advancedParametersLayout = new QFormLayout();
//    advancedParameters->setLayout(advancedParametersLayout);

//    createFastParameterBox(advancedParametersLayout);
//    createMserParameterBox(advancedParametersLayout);
//    createDbscanParameterBox(advancedParametersLayout);

//    mainLayout->addWidget(advancedParameters);

//    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
//    buttonBox->setObjectName( "buttonBox" );

//    connect(buttonBox, &QDialogButtonBox::accepted, this, &TrainingSamplesWindow::ok);
//    connect(buttonBox, &QDialogButtonBox::rejected, this, &TrainingSamplesWindow::cancel);

//    mainLayout->addWidget(buttonBox);

//    this->setLayout(mainLayout);
}






QGroupBox * TrainingSamplesWindow::createColorsAndIntensityBox()
{

    QGroupBox * groupBoxColor = new QGroupBox( tr("Colors and Intensity") );

    QFormLayout * colorLayout = new QFormLayout();

    groupBoxColor->setLayout( colorLayout );

    bgcolorLineEdit = new QLineEdit();
    bgcolorLineEdit->setAlignment(Qt::AlignRight);
    bgcolorLineEdit->setText( QString::number( parameters.bgcolor) );
    colorLayout->addRow( new QLabel(tr("Background color (currently grayscale images are assumed)")), bgcolorLineEdit);

    bgthreshLineEdit = new QLineEdit();
    bgthreshLineEdit->setAlignment(Qt::AlignRight);
    bgthreshLineEdit->setText( QString::number( parameters.bgthreshold) );
    colorLayout->addRow( new QLabel(tr("Background Color Threshold")), bgthreshLineEdit);

    // TODO combo box

    QStringListModel * model = new QStringListModel(this);

    QStringList comboOptions;

    comboOptions << tr( "No" ) << tr( "Yes" ) << tr( "Random" );

    model->setStringList( comboOptions );

    colorsInversionComboBox = new QComboBox(this);
    colorsInversionComboBox->setModel( model );
    colorsInversionComboBox->setEditable( false );

    colorLayout->addRow( new QLabel(tr("Colors inversion")), colorsInversionComboBox);

    maxidevLineEdit = new QLineEdit();
    maxidevLineEdit->setAlignment(Qt::AlignRight);
    maxidevLineEdit->setText( QString::number( parameters.maxintensitydev) );
    colorLayout->addRow( new QLabel(tr("Maximal Intensity Deviation of Pixels in Foreground Samples")), maxidevLineEdit);

    return groupBoxColor;

}












//void TrainingSamplesWindow::createFastParameterBox(QFormLayout * advancedParametersLayout){
//    fastThreshold = new QLineEdit();
//    fastThreshold->setValidator( new QIntValidator(1, 1000000, this) );
//    fastThreshold->setAlignment(Qt::AlignRight);
//    fastThreshold->setText(QString::fromStdString(std::to_string(fastThresholdValue)));

//    advancedParametersLayout->addRow(new QLabel(tr("FAST Corner Intensity Threshold")),fastThreshold);

//    fastType = new QComboBox(this);
//    //fastType->lineEdit()->setAlignment(Qt::AlignRight);
//    fastType->addItem(tr("5-8"),cv::FastFeatureDetector::TYPE_5_8);
//    fastType->addItem(tr("7-12"),cv::FastFeatureDetector::TYPE_7_12);
//    fastType->addItem(tr("9-16"),cv::FastFeatureDetector::TYPE_9_16);
//    fastType->setEditable(false);
//    fastType->setCurrentIndex(fastType->findData(fastTypeValue));

//    advancedParametersLayout->addRow(new QLabel(tr("FAST Corner Type")),fastType);

//    fastNonMaxSuppression = new QCheckBox(this);
//    advancedParametersLayout->addRow(new QLabel("FAST Use Non-maximal Suppression"),fastNonMaxSuppression);
//}

//void TrainingSamplesWindow::createDbscanParameterBox(QFormLayout * advancedParametersLayout){
//    dbscanEpsilon = new QLineEdit();
//    dbscanEpsilon->setValidator( new QIntValidator(1, 1000000, this) );
//    dbscanEpsilon->setAlignment(Qt::AlignRight);
//    dbscanEpsilon->setText(QString::fromStdString(std::to_string(dbscanEpsilonValue)));

//    advancedParametersLayout->addRow(new QLabel(tr("DBSCAN Epsilon")),dbscanEpsilon);

//    dbscanMinimumPointCount = new QLineEdit();
//    dbscanMinimumPointCount->setValidator( new QIntValidator(1, 1000000, this) );
//    dbscanMinimumPointCount->setAlignment(Qt::AlignRight);
//    dbscanMinimumPointCount->setText(QString::fromStdString(std::to_string(dbscanMinPointsValue)));

//    advancedParametersLayout->addRow(new QLabel(tr("DBSCAN Minimum Point Count")),dbscanMinimumPointCount);
//}


//void TrainingSamplesWindow::createMserParameterBox(QFormLayout * advancedParametersLayout){
//    mserDelta = new QLineEdit();
//    mserDelta->setValidator( new QIntValidator(1, 1000000, this) );
//    mserDelta->setAlignment(Qt::AlignRight);
//    mserDelta->setText(QString::fromStdString(std::to_string(mserDeltaValue)));

//    advancedParametersLayout->addRow(new QLabel(tr("MSER Delta")),mserDelta);

//    mserMinimumArea = new QLineEdit();
//    mserMinimumArea->setValidator( new QIntValidator(1, 1000000, this) );
//    mserMinimumArea->setAlignment(Qt::AlignRight);
//    mserMinimumArea->setText(QString::fromStdString(std::to_string(mserMinimumAreaValue)));

//    advancedParametersLayout->addRow(new QLabel(tr("MSER Minimum Area")),mserMinimumArea);

//    mserMaximumArea = new QLineEdit();
//    mserMaximumArea->setValidator( new QIntValidator(1, 1000000, this) );
//    mserMaximumArea->setAlignment(Qt::AlignRight);
//    mserMaximumArea->setText(QString::fromStdString(std::to_string(mserMaximumAreaValue)));

//    advancedParametersLayout->addRow(new QLabel(tr("MSER Maximum Area")),mserMaximumArea);

//}

//QGroupBox * TrainingSamplesWindow::createDisplayParameterBox(){
//    displayParameters = new QGroupBox(tr("Display"));

//    QVBoxLayout * displayParameterLayout = new QVBoxLayout();
//    displayParameters->setLayout(displayParameterLayout);

//    showFeatureMarkers = new QCheckBox(tr("Show microfeature markers"),this);
//    showFeatureMarkers->setChecked(showFeatureMarkersValue);
//    displayParameterLayout->addWidget(showFeatureMarkers);


//    mergeBoundingBoxes = new QCheckBox(tr("Merge overlapping bounding boxes"),this);
//    mergeBoundingBoxes->setChecked(mergeOverlappingBoundingBoxesValue);
//    displayParameterLayout->addWidget(mergeBoundingBoxes);


//    return displayParameters;
//}

//void TrainingSamplesWindow::ok(){
//    std::string sFastThreshold = fastThreshold->text().toStdString();

//    if(!sFastThreshold.empty()){
//        fastThresholdValue = std::atoi(sFastThreshold.c_str());

//        if(fastThresholdValue > 0){
//                fastTypeValue = fastType->currentData().toInt();

//                if(fastTypeValue > 0){

//                    std::string sDbscanEpsilon = dbscanEpsilon->text().toStdString();

//                    if(!sDbscanEpsilon.empty()){
//                        dbscanEpsilonValue = std::atoi(sDbscanEpsilon.c_str());

//                        if(dbscanEpsilonValue > 0){
//                            std::string sDbscanMinPoints = dbscanMinimumPointCount->text().toStdString();

//                            if(!sDbscanMinPoints.empty()){
//                                dbscanMinPointsValue = std::atoi(sDbscanMinPoints.c_str());

//                                if(dbscanMinPointsValue > 0){
//                                    std::string sMserDelta = mserDelta->text().toStdString();

//                                    if(!sMserDelta.empty()){
//                                        mserDeltaValue = std::atoi(sMserDelta.c_str());

//                                        if(mserDeltaValue > 0){
//                                            std::string sMserMinimumArea = mserMinimumArea->text().toStdString();

//                                            if(!sMserMinimumArea.empty()){
//                                                mserMinimumAreaValue = std::atoi(sMserMinimumArea.c_str());

//                                                std::string sMserMaximumArea = mserMaximumArea->text().toStdString();

//                                                if(!sMserMaximumArea.empty()){
//                                                    mserMaximumAreaValue = std::atoi(sMserMaximumArea.c_str());

//                                                    QProgressDialog progress("Finding objects...", QString(), 0, files.size(), this);

//                                                    progress.setWindowModality(Qt::WindowModal);
//                                                    progress.setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint | Qt::WindowTitleHint);

//                                                    progress.setValue(0);
//                                                    progress.setMinimumDuration( 0 );


//                                                    fastNonMaxSuppressionValue = fastNonMaxSuppression->isChecked();
//                                                    showFeatureMarkersValue = showFeatureMarkers->isChecked();
//                                                    mergeOverlappingBoundingBoxesValue = mergeBoundingBoxes->isChecked();


//                                                    QThread * workerThread = new QThread( this );

//                                                    WorkerDetection * worker = new WorkerDetection( this );

//                                                    worker->moveToThread(workerThread);

//                                                    connect( workerThread, &QThread::finished, worker, &WorkerDetection::deleteLater );
//                                                    connect( workerThread, &QThread::started, worker, &WorkerDetection::doWork );

//                                                    connect( worker, &WorkerDetection::progress, &progress, &QProgressDialog::setValue);

//                                                    workerThread->start();

//                                                    progress.exec();

//                                                    workerThread->quit();
//                                                    workerThread->wait();


//                                                }
//                                            }
//                                        }
//                                    }
//                                }
//                            }
//                        }
//                    }
//                    else{
//                        //TODO: whine about missing DBSCAN Epsilon
//                    }
//                }
//                else{
//                    //TODO: whine about bad FAST Type
//                }
//        }
//        else{
//            //TODO: message box whining about bad threshold
//        }
//    }
//    else{
//        //TODO: whine about missing threshold
//    }


//    this->accept();
//}

//void TrainingSamplesWindow::cancel(){
//    this->reject();
//}
