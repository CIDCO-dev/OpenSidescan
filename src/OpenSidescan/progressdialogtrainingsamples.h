/*
* Copyright 2019 © Centre Interdisciplinaire de développement en Cartographie des Océans (CIDCO), Tous droits réservés
*/

 /*
 * \author Christian Bouchard
 */

#ifndef PROGRESSDIALOGTRAININGSAMPLES_H
#define PROGRESSDIALOGTRAININGSAMPLES_H



#include <QDebug>

#include <QProgressDialog>


class ProgressDialogTrainingSamples : public QProgressDialog
{
    Q_OBJECT

public:
    ProgressDialogTrainingSamples(const QString &labelText,
                                  const QString &cancelButtonText,
                                  int minimum,
                                  int maximum,
                                  QWidget *parent = nullptr,
                                  Qt::WindowFlags f = Qt::WindowFlags())
        : QProgressDialog( labelText, cancelButtonText, minimum, maximum, parent, f)
    {

    }


//    virtual ~ProgressDialogTrainingSamples(){}




public slots:

//    virtual void cancel() override;

    void closeWindow()
    {
        QDialog::done( 0 );
    }




signals:
//    void canceled() override;

private:


};


#endif // PROGRESSDIALOGTRAININGSAMPLES_H

