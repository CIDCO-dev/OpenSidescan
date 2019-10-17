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

#include "sidescanfile.h"
#include "sidescanimage.h"
#include "opencvhelper.h"

class DetectionWindow : public QDialog
{
public:
    DetectionWindow(std::vector<SidescanFile *> & files,
                    int & fastThresholdValue,
                    int & fastTypeValue,
                    bool & fastNonMaxSuppressionValue,
                    int & dbscanEpsilonValue,
                    int & dbscanMinPointsValue,
                    int & mserDeltaValue,
                    int & mserMinimumAreaValue,
                    int & mserMaximumAreaValue,
                    double & mserMaximumVariationValue,
                    double & mserMinimumDiversityValue,
                    int & mserMaximumEvolutionValue,
                    double & mserAreaThresholdValue,
                    double & mserMinimumMarginValue,
                    int & mserEdgeBlurValue,
                    bool & showFeatureMarkersValue,
                    bool & mergeOverlappingBoundingBoxesValue,
                    QWidget * parent=0
    );

private slots:
    void ok();
    void cancel();

private:
    Q_OBJECT;

    std::vector<SidescanFile *>  & files;

    std::vector<GeoreferencedObject*> & objects;

    void createFastParameterBox(QFormLayout * advancedParametersLayout);
    void createMserParameterBox(QFormLayout * advancedParametersLayout);
    void createDbscanParameterBox(QFormLayout * advancedParametersLayout);
    QGroupBox * createDisplayParameterBox();

    QDialogButtonBox * buttonBox;

    //display parameters
    QGroupBox * displayParameters;

    QCheckBox * showFeatureMarkers;
    QCheckBox * mergeBoundingBoxes;

    //FAST parameters
    QGroupBox * advancedParameters;

    QLineEdit * fastThreshold;
    QComboBox * fastType;
    QCheckBox * fastNonMaxSuppression;

    //MSER parameters


    QLineEdit * mserDelta;
    QLineEdit * mserMinimumArea;
    QLineEdit * mserMaximumArea;
    QLineEdit * mserMaximumVariation;
    QLineEdit * mserMinimumDiversity;
    QLineEdit * mserMaximumEvolution;
    QLineEdit * mserAreaThreshold;
    QLineEdit * mserMinimumMargin;
    QLineEdit * mserEdgeBlur;

    //DBSCAN parameters;


    QLineEdit * dbscanEpsilon;
    QLineEdit * dbscanMinimumPointCount;



    //Values
    int & fastThresholdValue;
    int & fastTypeValue;
    bool & fastNonMaxSuppressionValue;
    int & dbscanEpsilonValue;
    int & dbscanMinPointsValue;
    int & mserDeltaValue;
    int & mserMinimumAreaValue;
    int & mserMaximumAreaValue;
    double & mserMaximumVariationValue;
    double & mserMinimumDiversityValue;
    int & mserMaximumEvolutionValue;
    double & mserAreaThresholdValue;
    double & mserMinimumMarginValue;
    int & mserEdgeBlurValue;
    bool & showFeatureMarkersValue;
    bool & mergeOverlappingBoundingBoxesValue;

    void initUI();

};

#endif // DETECTIONWINDOW_H
