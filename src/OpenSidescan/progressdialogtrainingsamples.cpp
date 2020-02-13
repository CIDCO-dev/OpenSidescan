/*
* Copyright 2019 © Centre Interdisciplinaire de développement en Cartographie des Océans (CIDCO), Tous droits réservés
*/

 /*
 * \author Christian Bouchard
 */
#include <QDebug>
#include <QString>
#include <QLabel>
#include <QDialogButtonBox>

#include <QVBoxLayout>

#include "progressdialogtrainingsamples.h"


ProgressDialogTrainingSamples::ProgressDialogTrainingSamples(
                              int minimum,
                              int maximum,
                              bool * continueWhatYourDoing,
                              QWidget *parent )
    : QDialog( parent ),
      continueWhatYourDoing( continueWhatYourDoing )
{
    QVBoxLayout *layout = new QVBoxLayout;

    QLabel * label = new QLabel( "Creating and Saving Training Object Samples..." );
    layout->addWidget( label );

    bar = new QProgressBar( this );
    bar->setMinimum( minimum );
    bar->setMaximum( maximum );
    bar->setValue( minimum );

    layout->addWidget( bar );


    cancelButton = new QPushButton( "Stop" );


//    layout->addWidget( cancelButton );


    QDialogButtonBox * buttonBox= new QDialogButtonBox(Qt::Horizontal);

    buttonBox->addButton(cancelButton, QDialogButtonBox::ActionRole);

    layout->addWidget( buttonBox );


    connect( cancelButton, SIGNAL( clicked() ), this, SLOT( cancelButtonClicked() ) );

    setLayout(layout);

}


void ProgressDialogTrainingSamples::cancelButtonClicked()
{
    qDebug() << tr( "ProgressDialogTrainingSamples::cancelButtonClicked()" );


    cancelButton->setEnabled( false );

    cancelButton->setText( "Stopping..." );

    // Change progress bar to a busy indicator
    bar->setMinimum( 0 );
    bar->setMaximum( 0 );
    bar->setValue( 0 );

    qDebug() << tr( "ProgressDialogTrainingSamples::cancelButtonClicked(), Before '*continueWhatYourDoing = false'" );


    *continueWhatYourDoing = false;

    qDebug() << tr( "End of ProgressDialogTrainingSamples::cancelButtonClicked()" );

}


void ProgressDialogTrainingSamples::setValue( int value )
{
    bar->setValue( value );
}


void ProgressDialogTrainingSamples::closeDialog()
{
//    qDebug() << tr( "ProgressDialogTrainingSamples::closeDialog()" );

    emit QDialog::done( 0 );

}
