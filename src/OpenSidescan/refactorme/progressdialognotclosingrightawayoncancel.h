/*
* Copyright 2019 © Centre Interdisciplinaire de développement en Cartographie des Océans (CIDCO), Tous droits réservés
*/

 /*
 * \author Christian Bouchard
 */

#ifndef PROGRESSDIALOGTRAININGSAMPLES_H
#define PROGRESSDIALOGTRAININGSAMPLES_H

#include <QDialog>
#include <QProgressBar>
#include <QPushButton>

#include "boolwithmutex.h"

class ProgressDialogNotClosingRightAwayOnCancel : public QDialog
{
    Q_OBJECT

public:
    ProgressDialogNotClosingRightAwayOnCancel( const QString &labelText,
                                   const QString &cancelButtonText,
                                   const QString &cancelButtonTextWhileCancelling,
                                   const int minimum,
                                  const int maximum,
                                   BoolWithMutex * continueWhatYourDoing,
                                   const bool doChangeToBusyIndicatorWhileCancelling = true,
                                  QWidget *parent = nullptr );

//    virtual ~ProgressDialogTrainingSamples(){}

public slots:

    void closeDialog();

    void cancelButtonClicked();

    void setValue( int value );


signals:

private:

    QProgressBar * bar;

    QPushButton * cancelButton;

    const QString cancelButtonTextWhileCancelling;

    BoolWithMutex * continueWhatYourDoing;

    const bool doChangeToBusyIndicatorWhileCancelling;


};


#endif // PROGRESSDIALOGTRAININGSAMPLES_H

