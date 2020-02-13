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

class ProgressDialogTrainingSamples : public QDialog
{
    Q_OBJECT

public:
    ProgressDialogTrainingSamples( int minimum,
                                  int maximum,
//                                  bool * continueWhatYourDoing,
                                   BoolWithMutex * continueWhatYourDoing,
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

    BoolWithMutex * continueWhatYourDoing;
};


#endif // PROGRESSDIALOGTRAININGSAMPLES_H

