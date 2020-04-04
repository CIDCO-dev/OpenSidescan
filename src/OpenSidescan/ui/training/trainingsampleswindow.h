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

#include <QStringListModel>




#include "parameterscvCreateTrainingSamples.h"

class TrainingSamplesWindow : public QDialog
{
    Q_OBJECT

public:
    TrainingSamplesWindow( QWidget *parent, const QString & folder,
                           const ParameterscvCreateTrainingSamples & parameters );

    virtual ~TrainingSamplesWindow();

    bool getUserDidCancel();

    void getFolder( QString & folderOUT );

    void getParameters( ParameterscvCreateTrainingSamples & parametersOUT );


public slots:

    void reject() override; // Respond to Escape key


    void cancelButtonClicked();
    void OKButtonClicked();

    void pathBrowseButtonClicked();

    void useOriginalWidthAsBasisCheckBoxStateChanged( int param );
    void useOriginalHeightAsBasisCheckBoxStateChanged( int param );




private:

    void initUI();

    QGroupBox * createColorsAndIntensityBox();
    QGroupBox * createMaxRotationBox();

    QGroupBox * createWidthBox();
    QGroupBox * createHeightBox();


    bool validateLineEditValues();


    void displayWarning( std::string & text );


    void updateValues();

    bool createFolders();



    QString folder;

    ParameterscvCreateTrainingSamples parameters;


    bool userDidCancel;

    const std::string originalObjectImages;
    const std::string outputPositiveSamples;
    const std::string background;


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


    QCheckBox * useOriginalWidthAsBasisCheckBox;
    QLabel * widthLabel;
    QLineEdit * widthLineEdit;


    QCheckBox * useOriginalHeightAsBasisCheckBox;
    QLabel * heightLabel;
    QLineEdit * heightLineEdit;


    QDialogButtonBox * buttonBox;













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
