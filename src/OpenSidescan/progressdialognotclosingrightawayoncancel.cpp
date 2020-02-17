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

#include "progressdialognotclosingrightawayoncancel.h"


ProgressDialogNotClosingRightAwayOnCancel::ProgressDialogNotClosingRightAwayOnCancel(
                               const QString &labelText,
                               const QString &cancelButtonText,
                               const QString &cancelButtonTextWhileCancelling,
                              const int minimum,
                              const int maximum,
                                BoolWithMutex * continueWhatYourDoing,
                                const bool doChangeToBusyIndicatorWhileCancelling,
                              QWidget *parent )
    : QDialog( parent ),
      cancelButtonTextWhileCancelling( cancelButtonTextWhileCancelling ),
      continueWhatYourDoing( continueWhatYourDoing ),
      doChangeToBusyIndicatorWhileCancelling( doChangeToBusyIndicatorWhileCancelling )
{
    QVBoxLayout *layout = new QVBoxLayout;

    QLabel * label = new QLabel( labelText );
    layout->addWidget( label );

    bar = new QProgressBar( this );
    bar->setMinimum( minimum );
    bar->setMaximum( maximum );
    bar->setValue( minimum );

    layout->addWidget( bar );


    cancelButton = new QPushButton( cancelButtonText );


//    layout->addWidget( cancelButton );


    QDialogButtonBox * buttonBox= new QDialogButtonBox(Qt::Horizontal);

    buttonBox->addButton(cancelButton, QDialogButtonBox::ActionRole);

    layout->addWidget( buttonBox );


    connect( cancelButton, &QPushButton::clicked,
             this, &ProgressDialogNotClosingRightAwayOnCancel::cancelButtonClicked );

    setLayout(layout);

}


void ProgressDialogNotClosingRightAwayOnCancel::cancelButtonClicked()
{
    cancelButton->setEnabled( false );

    cancelButton->setText( cancelButtonTextWhileCancelling );

    if ( doChangeToBusyIndicatorWhileCancelling ) {
        // Change progress bar to a busy indicator
        bar->setMinimum( 0 );
        bar->setMaximum( 0 );
        bar->setValue( 0 );
    }

    continueWhatYourDoing->setValue( false );
}


void ProgressDialogNotClosingRightAwayOnCancel::setValue( int value )
{
    bar->setValue( value );
}


void ProgressDialogNotClosingRightAwayOnCancel::closeDialog()
{
//    qDebug() << tr( "ProgressDialogNotClosingRightAwayOnCancel::closeDialog()" );

    emit QDialog::done( 0 );

}
