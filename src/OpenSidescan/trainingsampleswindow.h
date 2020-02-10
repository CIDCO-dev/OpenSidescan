#ifndef TRAININGSAMPLESWINDOW
#define TRAININGSAMPLESWINDOW



#include <QDialog>
#include <QDialogButtonBox>
//#include <QFormLayout>
#include <QGroupBox>
#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>
#include <QLabel>
//#include <QIntValidator>

#include <QStringListModel>


#include <QPalette>


#include "parameterscvCreateTrainingSamples.h"

class TrainingSamplesWindow : public QDialog
{
    Q_OBJECT

public:
    TrainingSamplesWindow( QWidget *parent, const QString & folder,
                           const ParameterscvCreateTrainingSamples & parameters );

    virtual ~TrainingSamplesWindow();

    bool getUserDidCancel();

//    void getFolder( QString & folder );

//    void getParameters( ParameterscvCreateTrainingSamples & parameters );


public slots:

    void reject() override; // Respond to Escape key

    void lineEditTextEdited( const QString &text );

    void cancelButtonClicked();
    void OKButtonClicked();

private:

    void initUI();

    QGroupBox * createColorsAndIntensityBox();
    QGroupBox * createMaxRotationBox();


    void setButtonsState();
    bool validateLineEditValues();
//    void updateValues();



    QString folder;

    ParameterscvCreateTrainingSamples parameters;


    bool userDidCancel;


//    QGroupBox * colorsGroupBox;
//    QGroupBox * rotationGroupBox;


    QLineEdit * pathLineEdit;

    QLineEdit * numLineEdit;

    QLineEdit * bgcolorLineEdit;
    QLineEdit * bgthreshLineEdit;

//    QStringListModel * model;
    QComboBox * colorsInversionComboBox;

    QLineEdit * maxidevLineEdit;

    QLineEdit * maxXdegreesLineEdit;
    QLineEdit * maxYdegreesLineEdit;
    QLineEdit * maxZdegreesLineEdit;


    QDialogButtonBox * buttonBox;




    QPalette * paletteLineEditDefault;
    QPalette * paletteLineEditRedBackgroud;








//    void createFastParameterBox(QFormLayout * advancedParametersLayout);
//    void createMserParameterBox(QFormLayout * advancedParametersLayout);
//    void createDbscanParameterBox(QFormLayout * advancedParametersLayout);
//    QGroupBox * createDisplayParameterBox();

//    QDialogButtonBox * buttonBox;

//    //display parameters
//    QGroupBox * displayParameters;

//    QCheckBox * showFeatureMarkers;
//    QCheckBox * mergeBoundingBoxes;

//    //FAST parameters
//    QGroupBox * advancedParameters;

//    QLineEdit * fastThreshold;
//    QComboBox * fastType;
//    QCheckBox * fastNonMaxSuppression;

//    //MSER parameters


//    QLineEdit * mserDelta;
//    QLineEdit * mserMinimumArea;
//    QLineEdit * mserMaximumArea;


//    //DBSCAN parameters;


//    QLineEdit * dbscanEpsilon;
//    QLineEdit * dbscanMinimumPointCount;



//    //Values
//    int & fastThresholdValue;
//    int & fastTypeValue;
//    bool & fastNonMaxSuppressionValue;
//    int & dbscanEpsilonValue;
//    int & dbscanMinPointsValue;
//    int & mserDeltaValue;
//    int & mserMinimumAreaValue;
//    int & mserMaximumAreaValue;
//    bool & showFeatureMarkersValue;
//    bool & mergeOverlappingBoundingBoxesValue;


};

#endif // TRAININGSAMPLESWINDOW
