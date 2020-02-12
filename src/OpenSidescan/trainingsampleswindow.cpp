

#include <iostream>

#include <QDebug>

#include <QPushButton>

#include <QFormLayout>
#include <QGridLayout>
//#include <QStringListModel>
#include <QIntValidator>

#include <QFileDialog>
#include <QMessageBox>

#include "../thirdParty/opencv/apps/createsamples/utility.hpp"

//#include "../thirdParty/MBES-lib/src/utils/Constants.hpp"
// TODO: M_PI is POSIX. Add #include <cmath> to MBES-lib's Constants.hpp so that M_PI will be defined
#define PI 3.14159265358979323846
#define R2D ((double)180/(double)PI)
#define D2R ((double)PI/(double)180)


#include "trainingsampleswindow.h"



TrainingSamplesWindow::TrainingSamplesWindow( QWidget *parent,
                                              const QString & folder,
                                 const ParameterscvCreateTrainingSamples & parameters )
    : QDialog( parent ),
      folder( folder ), parameters( parameters ),
      userDidCancel( false ),
      originalObjectImages( "OriginalObjectImages" ),
      outputPositiveSamples( "OutputPositiveSamples"),
      background("Background" )
{
    qDebug() << "Beginning of TrainingSamplesWindow::TrainingSamplesWindow()\n";

    std::cout << "this: " << this << "\n" << std::endl;

    initUI();

    qDebug() << "After call to initUI()\n";






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
    QGridLayout *layoutPath= new QGridLayout();

    mainLayout->addLayout( layoutPath );


    QLabel * pathLabel = new QLabel( tr( "Path" ) );

    pathLineEdit = new QLineEdit();
    pathLineEdit->setText( folder );

    pathLabel->setBuddy( pathLineEdit );

    QPushButton *pathBrowseButton = new QPushButton( "Browse" );

    layoutPath->addWidget( pathLabel, 0, 0 );
    layoutPath->addWidget( pathLineEdit, 0, 1 );
    layoutPath->addWidget( pathBrowseButton, 0, 2 );


    connect( pathBrowseButton, &QPushButton::clicked, this, &TrainingSamplesWindow::pathBrowseButtonClicked );



    qDebug() << "Before QFormLayout * numLayout = new QFormLayout();\n";

    // num
    QFormLayout * numLayout = new QFormLayout();

    mainLayout->addLayout( numLayout );

    numLineEdit = new QLineEdit();
    numLineEdit->setValidator( new QIntValidator(1, 1000000, this) );
    // TODO: the validator let me enter a value of 0
    // How to deal with that?

    numLineEdit->setAlignment(Qt::AlignRight);
    numLineEdit->setText( QString::number( parameters.num ) );

    numLayout->addRow( new QLabel(tr("Number of positive samples to create per object")), numLineEdit);


    qDebug() << "Before mainLayout->addWidget( createColorsAndIntensityBox() );\n";

    mainLayout->addWidget( createColorsAndIntensityBox() );



    qDebug() << "Before mainLayout->addWidget( createMaxRotationBox() );\n";

    mainLayout->addWidget( createMaxRotationBox() );


    qDebug() << "Before mainLayout->addWidget( createWidthBox() );\n";

    mainLayout->addWidget( createWidthBox() );


    qDebug() << "Before mainLayout->addWidget( createHeightBox() );\n";

    mainLayout->addWidget( createHeightBox() );


    qDebug() << "Before buttonBox\n";


    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    buttonBox->setObjectName( "buttonBox" );

    connect(buttonBox, &QDialogButtonBox::accepted, this, &TrainingSamplesWindow::OKButtonClicked);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &TrainingSamplesWindow::cancelButtonClicked);


    connect( useOriginalWidthAsBasisCheckBox, &QCheckBox::stateChanged,
             this, &TrainingSamplesWindow::useOriginalWidthAsBasisCheckBoxStateChanged );

    connect( useOriginalHeightAsBasisCheckBox, &QCheckBox::stateChanged,
             this, &TrainingSamplesWindow::useOriginalHeightAsBasisCheckBoxStateChanged );



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
    bgcolorLineEdit->setValidator( new QIntValidator(0, 255, this) );
    bgcolorLineEdit->setAlignment(Qt::AlignRight);
    bgcolorLineEdit->setText( QString::number( parameters.bgcolor) );
    colorLayout->addRow( new QLabel(tr("Background Color (currently grayscale images are assumed)")), bgcolorLineEdit);

    bgthreshLineEdit = new QLineEdit();
    bgthreshLineEdit->setValidator( new QIntValidator(0, 255, this) );
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

    int index = parameters.invert;
    if ( index == CV_RANDOM_INVERT )
        index = 2;

    colorsInversionComboBox->setCurrentIndex( index );



    colorLayout->addRow( new QLabel(tr("Colors inversion")), colorsInversionComboBox);

    maxidevLineEdit = new QLineEdit();
    maxidevLineEdit->setValidator( new QIntValidator(0, 255, this) );
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
    maxXdegreesLineEdit->setValidator( new QDoubleValidator(0, 180, 1, this) );
    maxXdegreesLineEdit->setAlignment(Qt::AlignRight);
    maxXdegreesLineEdit->setText( QString::number( parameters.maxxangle * R2D, 'f', 1 ) );
    rotationlayout->addRow( new QLabel(tr("Maximal rotation angle towards x-axis (deg)")), maxXdegreesLineEdit);

    maxYdegreesLineEdit = new QLineEdit();
    maxYdegreesLineEdit->setValidator( new QDoubleValidator(0, 180, 1, this) );
    maxYdegreesLineEdit->setAlignment(Qt::AlignRight);
    maxYdegreesLineEdit->setText( QString::number( parameters.maxyangle * R2D, 'f', 1 ) );
    rotationlayout->addRow( new QLabel(tr("Maximal rotation angle towards y-axis (deg)")), maxYdegreesLineEdit);

    maxZdegreesLineEdit = new QLineEdit();
    maxZdegreesLineEdit->setValidator( new QDoubleValidator(0, 180, 1, this) );
    maxZdegreesLineEdit->setAlignment(Qt::AlignRight);
    maxZdegreesLineEdit->setText( QString::number( parameters.maxzangle * R2D, 'f', 1 ) );
    rotationlayout->addRow( new QLabel(tr("Maximal rotation angle towards z-axis (deg)")), maxZdegreesLineEdit);

    qDebug() << "End of TrainingSamplesWindow::createMaxRotationBox()\n";

    return rotationGroupBox;
}





QGroupBox * TrainingSamplesWindow::createWidthBox()
{

    qDebug() << "Beginning of TrainingSamplesWindow::createWidthBox()\n";

    QGroupBox * widthGroupBox = new QGroupBox( tr("Width") );


    QVBoxLayout * widthLayout = new QVBoxLayout();

    widthGroupBox->setLayout( widthLayout );

    widthLabel = new QLabel( tr("Number of Pixels Change From Object Image Width") );


    useOriginalWidthAsBasisCheckBox = new QCheckBox(tr("Use Original Object Image Width as Basis"),this);
    useOriginalWidthAsBasisCheckBox->setChecked( parameters.useOriginalObjectImageWidthAsBasis );
    widthLayout->addWidget(useOriginalWidthAsBasisCheckBox);


    QFormLayout * widthFormLayout = new QFormLayout();

    widthLayout->addLayout( widthFormLayout );


    widthLineEdit = new QLineEdit();
    widthLineEdit->setValidator( new QIntValidator(-1000000, 1000000, this) );
    widthLineEdit->setAlignment(Qt::AlignRight);
    widthLineEdit->setText( QString::number( parameters.nbPixelsChangeFromObjectImageWidth ) );

    widthFormLayout->addRow( widthLabel, widthLineEdit);

    return widthGroupBox;
}

QGroupBox * TrainingSamplesWindow::createHeightBox()
{

    qDebug() << "Beginning of TrainingSamplesWindow::createHeightBox()\n";

    QGroupBox * heightGroupBox = new QGroupBox( tr("Height") );


    QVBoxLayout *heightLayout = new QVBoxLayout();

    heightGroupBox->setLayout( heightLayout );

    heightLabel = new QLabel( tr("Number of Pixels Change From Object Image Height") );


    useOriginalHeightAsBasisCheckBox = new QCheckBox(tr("Use Original Object Image Height as Basis"),this);
    useOriginalHeightAsBasisCheckBox->setChecked( parameters.useOriginalObjectImageHeightAsBasis );
    heightLayout->addWidget(useOriginalHeightAsBasisCheckBox);


    QFormLayout * heightFormLayout = new QFormLayout();

    heightLayout->addLayout( heightFormLayout );


    heightLineEdit = new QLineEdit();
    heightLineEdit->setValidator( new QIntValidator(-1000000, 1000000, this) );
    heightLineEdit->setAlignment(Qt::AlignRight);
    heightLineEdit->setText( QString::number( parameters.nbPixelsChangeFromObjectImageHeight ) );

    heightFormLayout->addRow( heightLabel, heightLineEdit);

    return heightGroupBox;
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

void TrainingSamplesWindow::pathBrowseButtonClicked()
{
    QFileDialog dialog( this,
                         tr( "Path"), pathLineEdit->text(),
                        tr( "All Files (*)") );


    dialog.setFileMode( QFileDialog::Directory ); // Get a single existing file
    dialog.setLabelText( QFileDialog::Accept, tr( "Select" ) ) ; // Name of the button, to replace the default "Open"

    dialog.setViewMode( QFileDialog::Detail );
    dialog.setOptions( QFileDialog::DontConfirmOverwrite );
    dialog.setOption( QFileDialog::ShowDirsOnly, true );

    QStringList fileNames;

    if ( dialog.exec() )
        fileNames = dialog.selectedFiles();

    if ( fileNames.size() > 0 )
    {
        QString fileName = fileNames.at( 0 );
        pathLineEdit->setText( fileName );
    }

}


void TrainingSamplesWindow::cancelButtonClicked()
{
    qDebug() << "In TrainingSamplesWindow::cancelButtonClicked()\n";

    userDidCancel = true;

//    emit QDialog::done( 0 );
    QDialog::reject();

}

void TrainingSamplesWindow::OKButtonClicked()
{


    qDebug() << "In TrainingSamplesWindow::OKButtonClicked()\n";

    if ( validateLineEditValues() == true )
    {
        if ( createFolders() == false ) {
            return;
        }

        updateValues();
//        emit QDialog::done( 0 );
        accept();
    }

//    // TODO: remove done() from here, keep it only if validation is true
//    emit QDialog::done( 0 );

}

void TrainingSamplesWindow::displayWarning( std::string & text )
{
    qDebug() << tr( text.c_str() );

    QMessageBox::warning( this, tr("Warning"), tr( text.c_str() ), QMessageBox::Ok );


}

bool TrainingSamplesWindow::validateLineEditValues()
{
    qDebug() << "In TrainingSamplesWindow::validateLineEditValues()\n";

//    bool allOK = true;

    bool OK = true;

    QString text;
    int numberInt;
    double numberDouble;


    // Validate that the path exists and is a folder

    QFileInfo fileInfo( pathLineEdit->text() );

    std::string path = pathLineEdit->text().toLocal8Bit().constData();

    if ( fileInfo.exists() == false || fileInfo.isDir() == false )
    {
        std::string toDisplay = "Invalid path\n\n\"" + path + "\"\n";
        displayWarning( toDisplay );
        return false;
    }


    fileInfo.setFile( pathLineEdit->text() + "/" + tr( originalObjectImages.c_str() ) );

    if ( fileInfo.exists() ) {
        std::string toDisplay = "\"" + originalObjectImages
                + "\" already exists. Please choose a different path where to write the file structure\n";
        displayWarning( toDisplay );
        return false;
    }


    fileInfo.setFile( pathLineEdit->text() + "/" + tr( outputPositiveSamples.c_str() ) );

    if ( fileInfo.exists() ) {
        std::string toDisplay = "\"" + outputPositiveSamples
                + "\" already exists. Please choose a different path where to write the file structure\n";
        displayWarning( toDisplay );
        return false;
    }

    fileInfo.setFile( pathLineEdit->text() + "/" + tr( background.c_str() ) );

    if ( fileInfo.exists() ) {
        std::string toDisplay = "\"" + background
                + "\" already exists. Please choose a different path where to write the file structure\n";
        displayWarning( toDisplay );
        return false;
    }



    qDebug() << "Before text = numLineEdit->text();\n";

    text = numLineEdit->text();

    qDebug() << "After text = numLineEdit->text();\n";

    qDebug() << "text:" << text;

    numberInt = text.toInt( &OK );

    if ( OK == false ) {
        std::string toDisplay = "Could not convert the Number of positive samples to create per object into an integer number.\n";
        displayWarning( toDisplay );
        return false;
    } else if ( numberInt <= 0) {
        std::string toDisplay = "The Number of positive samples to create per object must be an integer number larger than zero.\n";
        displayWarning( toDisplay );
        return false;
    }



    text = bgcolorLineEdit->text();

    numberInt = text.toInt( &OK );

    if ( OK == false ) {
        std::string toDisplay = "Could not convert the Background Color into an integer number.\n";
        displayWarning( toDisplay );
        return false;
    } else if ( numberInt < 0 || numberInt > 255) {
        std::string toDisplay = "The Background Color must be an integer number within 0 to 255 inclusively.\n";
        displayWarning( toDisplay );
        return false;
    }


    text = bgthreshLineEdit->text();

    numberInt = text.toInt( &OK );

    if ( OK == false ) {
        std::string toDisplay = "Could not convert the Background Color Threshold into an integer number.\n";
        displayWarning( toDisplay );
        return false;
    } else if ( numberInt < 0 || numberInt > 255) {
        std::string toDisplay = "The Background Color Threshold must be an integer number within 0 to 255 inclusively.\n";
        displayWarning( toDisplay );
        return false;
    }


    // maxintensitydev value between 0 and 255 inclusively
    // Reference: Function "icvPlaceDistortedSample()" of file openv/apps/createsamples/utility.cpp
    // forecolordev = theRNG().uniform( -maxintensitydev, maxintensitydev );
    // uchar chartmp = (uchar) MAX( 0, MIN( 255, forecolordev + img.at<uchar>(r, c)) );

    text = maxidevLineEdit->text();

    numberInt = text.toInt( &OK );

    if ( OK == false ) {
        std::string toDisplay = "Could not convert the Maximal Intensity Deviation of Pixels in Foreground Samples into an integer number.\n";
        displayWarning( toDisplay );
        return false;
    } else if ( numberInt < 0 || numberInt > 255) {
        std::string toDisplay = "The Maximal Intensity Deviation of Pixels in Foreground Samples must be an integer number within 0 to 255 inclusively.\n";
        displayWarning( toDisplay );
        return false;
    }


    // Max angle text between 0 and 180 degrees inclusively
    // Reference: Function "icvRandomQuad()" of file openv/apps/createsamples/utility.cpp
    // uses an uniform distribution over the range -maxxangle to maxxangle.

    text = maxXdegreesLineEdit->text();

    numberDouble = text.toDouble( &OK );

    if ( OK == false ) {
        std::string toDisplay = "Could not convert the Maximal rotation angle towards x-axis (deg) into a double floating point number.\n";
        displayWarning( toDisplay );
        return false;
    } else if ( numberDouble < 0 || numberDouble > 180 ) {
        std::string toDisplay = "The Maximal rotation angle towards x-axis (deg) must be within 0 to 180 inclusively.\n";
        displayWarning( toDisplay );
        return false;
    }


    text = maxYdegreesLineEdit->text();

    numberDouble = text.toDouble( &OK );

    if ( OK == false ) {
        std::string toDisplay = "Could not convert the Maximal rotation angle towards y-axis (deg) into a double floating point number.\n";
        displayWarning( toDisplay );
        return false;
    } else if ( numberDouble < 0 || numberDouble > 180 ) {
        std::string toDisplay = "The Maximal rotation angle towards y-axis (deg) must be within 0 to 180 inclusively.\n";
        displayWarning( toDisplay );
        return false;
    }


    text = maxZdegreesLineEdit->text();

    numberDouble = text.toDouble( &OK );

    if ( OK == false ) {
        std::string toDisplay = "Could not convert the Maximal rotation angle towards z-axis (deg) into a double floating point number.\n";
        displayWarning( toDisplay );
        return false;
    } else if ( numberDouble < 0 || numberDouble > 180 ) {
        std::string toDisplay = "The Maximal rotation angle towards z-axis (deg) must be within 0 to 180 inclusively.\n";
        displayWarning( toDisplay );
        return false;
    }


    text = widthLineEdit->text();

    numberInt = text.toInt( &OK );

    if ( useOriginalWidthAsBasisCheckBox->isChecked() ) {

        if ( OK == false ) {
            std::string toDisplay = "Could not convert the Number of Pixels Change From Object Image Width into an integer number.\n";
            displayWarning( toDisplay );
            return false;
        }

        // The Number of Pixels Change can be positive or negative


    } else {

        if ( OK == false ) {
            std::string toDisplay = "Could not convert the Width (pixels) into an integer number.\n";
            displayWarning( toDisplay );
            return false;
        } else if ( numberInt <= 0  ) {
            std::string toDisplay = "The Width (pixels) must be an integer number larger than zero.\n";
            displayWarning( toDisplay );
            return false;
        }
    }


    text = heightLineEdit->text();

    numberInt = text.toInt( &OK );

    if ( useOriginalHeightAsBasisCheckBox->isChecked() ) {

        if ( OK == false ) {
            std::string toDisplay = "Could not convert the Number of Pixels Change From Object Image Height into an integer number.\n";
            displayWarning( toDisplay );
            return false;
        }

        // The Number of Pixels Change can be positive or negative


    } else {

        if ( OK == false ) {
            std::string toDisplay = "Could not convert the Height (pixels) into an integer number.\n";
            displayWarning( toDisplay );
            return false;
        } else if ( numberInt <= 0  ) {
            std::string toDisplay = "The Height (pixels) must be an integer number larger than zero.\n";
            displayWarning( toDisplay );
            return false;
        }
    }


    return true;
}


bool TrainingSamplesWindow::createFolders()
{

    QFileInfo fileInfo( pathLineEdit->text() );


    // Make the folder structure

    fileInfo.setFile( pathLineEdit->text() );

    // Try and create the folder in which to put the images
    QDir dir( fileInfo.absolutePath() );


    if ( ! dir.mkdir( pathLineEdit->text() + "/" + tr( originalObjectImages.c_str() ) ) )
    {
        std::string toDisplay = "Could not create the folder \n\n\""
                + originalObjectImages
                + "\"\n\nin the path \n\n\"" + pathLineEdit->text().toStdString() + "\"\n";

        qDebug() << tr( toDisplay.c_str() );

        QMessageBox::warning( this, tr("Warning"), tr( toDisplay.c_str() ), QMessageBox::Ok );

        return false;
    }

    if ( ! dir.mkdir( pathLineEdit->text() + "/" + tr( outputPositiveSamples.c_str() ) ) )
    {
        std::string toDisplay = "Could not create the folder \n\n\""
                + outputPositiveSamples
                + "\"\n\nin the path \n\n\"" + pathLineEdit->text().toStdString() + "\"\n";

        qDebug() << tr( toDisplay.c_str() );

        QMessageBox::warning( this, tr("Warning"), tr( toDisplay.c_str() ), QMessageBox::Ok );

        return false;
    }

    if ( ! dir.mkdir( pathLineEdit->text() + "/" + tr( background.c_str() ) ) )
    {
        std::string toDisplay = "Could not create the folder \n\n\""
                + background
                + "\"\n\nin the path \n\n\"" + pathLineEdit->text().toStdString() + "\"\n";

        qDebug() << tr( toDisplay.c_str() );

        QMessageBox::warning( this, tr("Warning"), tr( toDisplay.c_str() ), QMessageBox::Ok );

        return false;
    }

    return true;
}




void TrainingSamplesWindow::updateValues()
{
    folder = pathLineEdit->text();

    bool OK;

    parameters.num = numLineEdit->text().toInt( &OK );

    parameters.bgcolor = bgcolorLineEdit->text().toInt( &OK );

    parameters.bgthreshold = bgthreshLineEdit->text().toInt( &OK );


    parameters.invert = colorsInversionComboBox->currentIndex();

    if ( parameters.invert == 2 )
        parameters.invert = CV_RANDOM_INVERT;

    qDebug() << "TrainingSamplesWindow::updateValues(), parameters.invert: "
             << parameters.invert <<"\n";

    parameters.maxintensitydev = maxidevLineEdit->text().toInt( &OK );

    parameters.maxxangle = maxXdegreesLineEdit->text().toDouble( &OK ) * D2R;
    if ( parameters.maxxangle < 0 )
        parameters.maxxangle = 0;
    else if ( parameters.maxxangle > PI )
        parameters.maxxangle = PI;

    parameters.maxyangle = maxYdegreesLineEdit->text().toDouble( &OK ) * D2R;
    if ( parameters.maxyangle < 0 )
        parameters.maxyangle = 0;
    else if ( parameters.maxyangle > PI )
        parameters.maxyangle = PI;

    parameters.maxzangle = maxZdegreesLineEdit->text().toDouble( &OK ) * D2R;
    if ( parameters.maxzangle < 0 )
        parameters.maxzangle = 0;
    else if ( parameters.maxzangle > PI )
        parameters.maxzangle = PI;


    int numberInt;

    numberInt = widthLineEdit->text().toInt( &OK );

    parameters.useOriginalObjectImageWidthAsBasis = useOriginalWidthAsBasisCheckBox->isChecked();

    if ( parameters.useOriginalObjectImageWidthAsBasis ) {
        parameters.nbPixelsChangeFromObjectImageWidth = numberInt;
    } else {
        parameters.width = numberInt;
    }


    numberInt = heightLineEdit->text().toInt( &OK );

    parameters.useOriginalObjectImageHeightAsBasis = useOriginalHeightAsBasisCheckBox->isChecked();

    if ( parameters.useOriginalObjectImageHeightAsBasis ) {
        parameters.nbPixelsChangeFromObjectImageHeight = numberInt;
    } else {
        parameters.height = numberInt;
    }


}

void TrainingSamplesWindow::useOriginalWidthAsBasisCheckBoxStateChanged( int param )
{
    if ( useOriginalWidthAsBasisCheckBox->isChecked() ) {
        widthLabel->setText( tr("Number of Pixels Change From Object Image Width") );
        widthLineEdit->setText( QString::number( parameters.nbPixelsChangeFromObjectImageWidth ) );
        widthLineEdit->setValidator( new QIntValidator(-1000000, 1000000, this) );

    } else {
        widthLabel->setText( tr("Width (pixels)") );
        widthLineEdit->setText( QString::number( parameters.width ) );
        widthLineEdit->setValidator( new QIntValidator(1, 1000000, this) );
   }

}


void TrainingSamplesWindow::useOriginalHeightAsBasisCheckBoxStateChanged( int param )
{
    if ( useOriginalHeightAsBasisCheckBox->isChecked() ) {
        heightLabel->setText( tr("Number of Pixels Change From Object Image Height") );
        heightLineEdit->setText( QString::number( parameters.nbPixelsChangeFromObjectImageHeight ) );
        heightLineEdit->setValidator( new QIntValidator(-1000000, 1000000, this) );


    } else {
        heightLabel->setText( tr("Height (pixels)") );
        heightLineEdit->setText( QString::number( parameters.height ) );
        heightLineEdit->setValidator( new QIntValidator(1, 1000000, this) );
   }

}

void TrainingSamplesWindow::getFolder( QString & folderOUT )
{
    folderOUT = folder;
}

void TrainingSamplesWindow::getParameters( ParameterscvCreateTrainingSamples & parametersOUT )
{
    parametersOUT = parameters;
}
