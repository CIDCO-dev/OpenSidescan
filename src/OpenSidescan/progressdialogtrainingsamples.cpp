/*
* Copyright 2019 © Centre Interdisciplinaire de développement en Cartographie des Océans (CIDCO), Tous droits réservés
*/

 /*
 * \author Christian Bouchard
 */


#include <QVBoxLayout>

#include "progressdialogtrainingsamples.h"


ProgressDialogTrainingSamples::ProgressDialogTrainingSamples( const QString &labelText,
                              const QString &cancelButtonText,
                              int minimum,
                              int maximum,
                              bool * continueWhatYourDoing,
                              QWidget *parent )
    : QDialog( parent ),
      continueWhatYourDoing( continueWhatYourDoing )
{
    QVBoxLayout *layout = new QVBoxLayout;

    label = new QLabel( labelText );
    layout->addWidget( label );

    bar = new QProgressBar( this );
    bar->setMinimum( minimum );
    bar->setMaximum( maximum );
    bar->setValue( minimum );

    layout->addWidget( bar );


    cancelButton = new QPushButton( cancelButtonText);

    layout->addWidget( cancelButton );

    connect( cancelButton, SIGNAL( clicked() ), this, SLOT( cancelButtonClicked() ) );

    setLayout(layout);

}


void ProgressDialogTrainingSamples::cancelButtonClicked()
{
    qDebug() << tr( "ProgressDialogTrainingSamples::cancelButtonClicked()" );


    cancelButton->setEnabled( false );

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
