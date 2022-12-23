#ifndef DETECTIONWINDOW_H
#define DETECTIONWINDOW_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>
#include <QLabel>
#include <QIntValidator>
#include <QPushButton>

#include "sidescan/sidescanfile.h"
#include "sidescan/sidescanimage.h"
#include "utilities/opencvhelper.h"
#include "detector/roidetector.h"
#include "detector/yolov5detector.h"
#include "detector/houghdetector.h"
#include "project/project.h"


class DetectionWindow : public QDialog
{
    Q_OBJECT

public:
    DetectionWindow(Project & project,
                    int & fastThresholdValue,
                    cv::FastFeatureDetector::DetectorType & fastTypeValue,
                    bool & fastNonMaxSuppressionValue,
                    int & dbscanEpsilonValue,
                    int & dbscanMinPointsValue,
                    int & mserDeltaValue,
                    int & mserMinimumAreaValue,
                    int & mserMaximumAreaValue,
                    bool & mergeOverlappingBoundingBoxesValue,
                    QWidget * parent=0
    );

    int getFastThresholdValue() const { return fastThresholdValue; }
    int getFastTypeValue() const { return fastTypeValue; }
    bool getFastNonMaxSuppressionValue() const { return fastNonMaxSuppressionValue; }
    int getDbscanEpsilonValue() const { return dbscanEpsilonValue; }
    int getDbscanMinPointsValue() const { return dbscanMinPointsValue; }
    int getMserDeltaValue() const { return mserDeltaValue; }
    int getMserMinimumAreaValue() const { return mserMinimumAreaValue; }
    int getMserMaximumAreaValue() const { return mserMaximumAreaValue; }

    bool getMergeOverlappingBoundingBoxesValue() const { return mergeOverlappingBoundingBoxesValue; }


private slots:
    void ok();
    void cancel();
    void detectorChanged(int i);
    void selectModel();

private:
    void buildShipwreckDetector();
    void buildAdvancedDetector();
    void buildHoughDetector();
    void buildYolov5Detector();
    void buildGhostGearDetector();

    void launchDetectionWorker(Detector * detector);

    void createFastParameterBox(QFormLayout * advancedParametersLayout);
    void createMserParameterBox(QFormLayout * advancedParametersLayout);
    void createDbscanParameterBox(QFormLayout * advancedParametersLayout);
    void createYoloParameterBox(QGridLayout * yoloParametersLayout);

    Project & project;

    QGroupBox * createDisplayParameterBox();

    QDialogButtonBox * buttonBox;

    QComboBox * cmbDetector;
    int         currentDetectorIndex;

    //display parameters
    QGroupBox * displayParameters;

    QCheckBox * mergeBoundingBoxes;

    //YoloV5 parameters
    QGroupBox * yoloParameters;
    QPushButton *modelDialogButton;
    QLineEdit * modelPath;
    QLineEdit * scoresThreshold;
    QLineEdit * nmsThreshold;
    QLineEdit * confidenceThreshold;

    //FAST parameters
    QGroupBox * advancedParameters;

    QLineEdit * fastThreshold;
    QComboBox * fastType;
    QCheckBox * fastNonMaxSuppression;

    //MSER parameters
    QLineEdit * mserDelta;
    QLineEdit * mserMinimumArea;
    QLineEdit * mserMaximumArea;


    //DBSCAN parameters;
    QLineEdit * dbscanEpsilon;
    QLineEdit * dbscanMinimumPointCount;

    //Values
    int & fastThresholdValue;
    cv::FastFeatureDetector::DetectorType & fastTypeValue;
    bool & fastNonMaxSuppressionValue;
    int & dbscanEpsilonValue;
    int & dbscanMinPointsValue;
    int & mserDeltaValue;
    int & mserMinimumAreaValue;
    int & mserMaximumAreaValue;
    bool & mergeOverlappingBoundingBoxesValue;

    void initUI();
    
    unsigned long int imageCount;

};

#endif // DETECTIONWINDOW_H
