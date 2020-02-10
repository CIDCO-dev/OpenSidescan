

#include <iostream>

#include <QDebug>

#include <QPushButton>

#include <QFormLayout>
#include <QGridLayout>
//#include <QStringListModel>


#include "trainingsampleswindow.h"



TrainingSamplesWindow::TrainingSamplesWindow( QWidget *parent,
                                              const QString & folder,
                                 const ParameterscvCreateTrainingSamples & parameters )
    : QDialog( parent ),
      folder( folder ), parameters( parameters ),
      userDidCancel( false )
{
    qDebug() << "Beginning of TrainingSamplesWindow::TrainingSamplesWindow()\n";

    std::cout << "this: " << this << "\n" << std::endl;

    initUI();

    qDebug() << "After call to initUI()\n";


    // Palette to change background color of line edits having invalid values
    paletteLineEditDefault = new QPalette( numLineEdit->palette() );
    paletteLineEditRedBackgroud = new QPalette( numLineEdit->palette() );
    QColor red( 255, 102, 102 );
    paletteLineEditRedBackgroud->setColor(QPalette::Base, red );

    qDebug() << "paletteLineEditDefault: red: "
             << paletteLineEditDefault->color(QPalette::Window).red()
             << ", green: " << paletteLineEditDefault->color(QPalette::Window).green()
             << ", blue: " << paletteLineEditDefault->color(QPalette::Window).blue();

    qDebug() << "paletteLineEditDefault: red: "
             << paletteLineEditDefault->color(QPalette::Base).red()
             << ", green: " << paletteLineEditDefault->color(QPalette::Base).green()
             << ", blue: " << paletteLineEditDefault->color(QPalette::Base).blue();

    qDebug() << "paletteLineEditRedBackgroud: red: "
             << paletteLineEditRedBackgroud->color(QPalette::Base).red()
             << ", green: " << paletteLineEditRedBackgroud->color(QPalette::Base).green()
             << ", blue: " << paletteLineEditRedBackgroud->color(QPalette::Base).blue();

    qDebug() << "numLineEdit->styleSheet(): " << numLineEdit->styleSheet();

    qDebug() << "styleSheet(): " << styleSheet();



    qDebug() << "End of TrainingSamplesWindow::TrainingSamplesWindow()\n";

}


TrainingSamplesWindow::~TrainingSamplesWindow()
{
    qDebug() << "In TrainingSamplesWindow::~TrainingSamplesWindow()\n";
}

void TrainingSamplesWindow::initUI(){

    qDebug() << "Beginning of initUI()\n";

    setWindowTitle("Parameters to Create Positive Samples");





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

    qDebug() << "Before QGridLayout *layoutPath= new QGridLayout;\n";

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


    qDebug() << "Before QFormLayout * numLayout = new QFormLayout();\n";

    // num
    QFormLayout * numLayout = new QFormLayout();

    mainLayout->addLayout( numLayout );

    numLineEdit = new QLineEdit();

    numLineEdit->setAlignment(Qt::AlignRight);
    numLineEdit->setText( QString::number( parameters.num ) );

    numLayout->addRow( new QLabel(tr("Number of positive samples to create per object")), numLineEdit);


    qDebug() << "Before mainLayout->addWidget( createColorsAndIntensityBox() );\n";

    mainLayout->addWidget( createColorsAndIntensityBox() );



    qDebug() << "Before mainLayout->addWidget( createMaxRotationBox() );\n";

    mainLayout->addWidget( createMaxRotationBox() );


    qDebug() << "Before connect\n";


    connect( numLineEdit, &QLineEdit::textEdited, this, &TrainingSamplesWindow::lineEditTextEdited );






    qDebug() << "Before buttonBox\n";


    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    buttonBox->setObjectName( "buttonBox" );

    connect(buttonBox, &QDialogButtonBox::accepted, this, &TrainingSamplesWindow::OKButtonClicked);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &TrainingSamplesWindow::cancelButtonClicked);

    mainLayout->addWidget(buttonBox);


    setLayout(mainLayout);

    qDebug() << "End of initUI()\n";


}






QGroupBox * TrainingSamplesWindow::createColorsAndIntensityBox()
{
    qDebug() << "Beginning of TrainingSamplesWindow::createColorsAndIntensityBox()\n";

    QGroupBox * colorsGroupBox = new QGroupBox( tr("Colors and Intensity") );
//    colorsGroupBox = new QGroupBox( tr("Colors and Intensity") );


    QFormLayout * colorLayout = new QFormLayout();

    colorsGroupBox->setLayout( colorLayout );

    bgcolorLineEdit = new QLineEdit();
    bgcolorLineEdit->setAlignment(Qt::AlignRight);
    bgcolorLineEdit->setText( QString::number( parameters.bgcolor) );
    colorLayout->addRow( new QLabel(tr("Background color (currently grayscale images are assumed)")), bgcolorLineEdit);

    bgthreshLineEdit = new QLineEdit();
    bgthreshLineEdit->setAlignment(Qt::AlignRight);
    bgthreshLineEdit->setText( QString::number( parameters.bgthreshold) );
    colorLayout->addRow( new QLabel(tr("Background Color Threshold")), bgthreshLineEdit);

    // Combo box

    QStringListModel * model = new QStringListModel(this);
//    model = new QStringListModel(this);

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

    qDebug() << "End of TrainingSamplesWindow::createColorsAndIntensityBox()\n";


    return colorsGroupBox;

}



QGroupBox * TrainingSamplesWindow::createMaxRotationBox()
{

    qDebug() << "Beginning of TrainingSamplesWindow::createMaxRotationBox()\n";

    QGroupBox * rotationGroupBox = new QGroupBox( tr("Maximal Rotation Angles") );
//    rotationGroupBox = new QGroupBox( tr("Maximal Rotation Angles") );


    QFormLayout * rotationlayout = new QFormLayout();

    rotationGroupBox->setLayout( rotationlayout );

    // TODO Write function to convert angles

    maxXdegreesLineEdit = new QLineEdit();
    maxXdegreesLineEdit->setAlignment(Qt::AlignRight);
    maxXdegreesLineEdit->setText( QString::number( parameters.maxxangle * 180 / 3.1416, 'f', 1 ) );
    rotationlayout->addRow( new QLabel(tr("Maximal rotation angle towards x-axis (deg)")), maxXdegreesLineEdit);

    maxYdegreesLineEdit = new QLineEdit();
    maxYdegreesLineEdit->setAlignment(Qt::AlignRight);
    maxYdegreesLineEdit->setText( QString::number( parameters.maxyangle * 180 / 3.1416, 'f', 1 ) );
    rotationlayout->addRow( new QLabel(tr("Maximal rotation angle towards y-axis (deg)")), maxYdegreesLineEdit);

    maxZdegreesLineEdit = new QLineEdit();
    maxZdegreesLineEdit->setAlignment(Qt::AlignRight);
    maxZdegreesLineEdit->setText( QString::number( parameters.maxzangle * 180 / 3.1416, 'f', 1 ) );
    rotationlayout->addRow( new QLabel(tr("Maximal rotation angle towards z-axis (deg)")), maxZdegreesLineEdit);

    qDebug() << "End of TrainingSamplesWindow::createMaxRotationBox()\n";

    return rotationGroupBox;
}

bool TrainingSamplesWindow::getUserDidCancel()
{
     qDebug() << "Beginning of getUserDidCancel()\n";

    return userDidCancel;
}

void TrainingSamplesWindow::reject()
{
    qDebug() << "In TrainingSamplesWindow::reject()\n";

    userDidCancel = true;

    QDialog::reject();
}

void TrainingSamplesWindow::cancelButtonClicked()
{
    qDebug() << "In TrainingSamplesWindow::cancelButtonClicked()\n";

    userDidCancel = true;

    emit QDialog::done( 0 );
}

void TrainingSamplesWindow::OKButtonClicked()
{


    qDebug() << "In TrainingSamplesWindow::OKButtonClicked()\n";

//    if ( validateLineEditValues() == true )
//    {
//        updateValues();
//        emit QDialog::done( 0 );
//    }

    // TODO remove from here
    emit QDialog::done( 0 );

}

void TrainingSamplesWindow::lineEditTextEdited( const QString &text )
{
    qDebug() << "In TrainingSamplesWindow::lineEditTextEdited()\n";

    setButtonsState();
}

void TrainingSamplesWindow::setButtonsState()
{
    qDebug() << "In TrainingSamplesWindow::setButtonsState()\n";

    if ( validateLineEditValues() == true )
    {
        qDebug() << "In if of setButtonsState\n";

        buttonBox->button( QDialogButtonBox::Ok )->setEnabled( true );
    }
    else
    {
        qDebug() << "In else of setButtonsState\n";

        buttonBox->button( QDialogButtonBox::Ok )->setEnabled( false );
    }
}






bool TrainingSamplesWindow::validateLineEditValues()
{
    qDebug() << "In TrainingSamplesWindow::validateLineEditValues()\n";

    bool allOK = true;

    bool OK = true;

//    QLineEdit * numLineEdit;

//    QLineEdit * bgcolorLineEdit;
//    QLineEdit * bgthreshLineEdit;
//    QComboBox * colorsInversionComboBox;
//    QLineEdit * maxidevLineEdit;



//    QLineEdit * maxXdegreesLineEdit;
//    QLineEdit * maxYdegreesLineEdit;
//    QLineEdit * maxZdegreesLineEdit;

    QString text;
    int numberInt;
    double numberDouble;

    qDebug() << "Before text = numLineEdit->text();\n";

    text = numLineEdit->text();

    qDebug() << "After text = numLineEdit->text();\n";


    qDebug() << "test:" << text;





    numberInt = text.toInt( &OK );

    if( OK  && numberInt > 0 ) {
        qDebug() << "In if\n";
//        numLineEdit->setPalette( *paletteLineEditDefault );

        numLineEdit->setStyleSheet( "background-color: #19232D" );


    }
    else
    {
        qDebug() << "In else\n";

//        numLineEdit->setPalette( *paletteLineEditRedBackgroud );

        numLineEdit->setStyleSheet( "background-color: #FF6666" );

        allOK = false;
    }


//    text = numLineEdit->text();
//    numberInt = text.toInt( &OK );
//    if( OK)
//        numLineEdit->setPalette( *paletteLineEditDefault );
//    else
//    {
//        numLineEdit->setPalette( *paletteLineEditRedBackgroud );
//        allOK = false;
//    }




    return allOK;
}
