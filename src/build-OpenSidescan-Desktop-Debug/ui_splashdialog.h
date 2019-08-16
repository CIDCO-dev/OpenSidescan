/********************************************************************************
** Form generated from reading UI file 'splashdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SPLASHDIALOG_H
#define UI_SPLASHDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>

QT_BEGIN_NAMESPACE

class Ui_SplashDialog
{
public:
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *SplashDialog)
    {
        if (SplashDialog->objectName().isEmpty())
            SplashDialog->setObjectName(QStringLiteral("SplashDialog"));
        SplashDialog->setWindowModality(Qt::ApplicationModal);
        SplashDialog->resize(401, 300);
        SplashDialog->setModal(true);
        buttonBox = new QDialogButtonBox(SplashDialog);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(30, 240, 341, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        retranslateUi(SplashDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), SplashDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), SplashDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(SplashDialog);
    } // setupUi

    void retranslateUi(QDialog *SplashDialog)
    {
        SplashDialog->setWindowTitle(QApplication::translate("SplashDialog", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SplashDialog: public Ui_SplashDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SPLASHDIALOG_H
