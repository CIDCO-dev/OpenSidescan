#ifndef PROJECTPROPERTIESWINDOW_H
#define PROJECTPROPERTIESWINDOW_H

#include <QObject>
#include <QDialog>
#include <QFormLayout>
#include <QGroupBox>
#include <QLineEdit>
#include <QDoubleValidator>
#include <QLabel>
#include <QDialogButtonBox>

#include "project.h"

class ProjectPropertiesWindow : public QDialog
{
    Q_OBJECT

public:
    ProjectPropertiesWindow(QWidget * parent);

private slots:
    void ok();
    void cancel();

private:
    void initUI();

    void createLeverArmBox(QFormLayout * leverArmLayout);

    QDialogButtonBox * buttonBox;

    QGroupBox * leverArmBox;

    QLineEdit * xLeverArm;
    QLineEdit * yLeverArm;
    QLineEdit * zLeverArm;

    Project *   project            = NULL;
};

#endif // PROJECTPROPERTIESWINDOW_H
