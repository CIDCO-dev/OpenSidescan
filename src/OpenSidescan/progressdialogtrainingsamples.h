/*
* Copyright 2019 © Centre Interdisciplinaire de développement en Cartographie des Océans (CIDCO), Tous droits réservés
*/

 /*
 * \author Christian Bouchard
 */

#ifndef PROGRESSDIALOGTRAININGSAMPLES_H
#define PROGRESSDIALOGTRAININGSAMPLES_H



#include <QDebug>

#include <QDialog>
#include <QString>
#include <QLabel>
#include <QProgressBar>
#include <QPushButton>



class ProgressDialogTrainingSamples : public QDialog
{
    Q_OBJECT

public:
    ProgressDialogTrainingSamples( const QString &labelText,
                                  const QString &cancelButtonText,
                                  int minimum,
                                  int maximum,
                                  QWidget *parent = nullptr );



//    virtual ~ProgressDialogTrainingSamples(){}




public slots:

    void closeDialog();

    void cancelButtonClicked();

    void setValue( int value );



signals:

private:

    QLabel * label;
    QProgressBar * bar;

    QPushButton * cancelButton;

    bool * continueWhatYourDoing;

};


#endif // PROGRESSDIALOGTRAININGSAMPLES_H

