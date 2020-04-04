#ifndef VESSELGEOMETRYWINDOW_H
#define VESSELGEOMETRYWINDOW_H

#include <QObject>
#include <QDockWidget>
#include <QFormLayout>
#include <QGroupBox>
#include <QLineEdit>
#include <QDoubleValidator>
#include <QLabel>
#include <QDialogButtonBox>

#include "project/project.h"

class VesselGeometryWindow : public QDockWidget
{
    Q_OBJECT

public:
    VesselGeometryWindow(QWidget * parent);

    void setProject(Project * p);

private slots:
    void editingFinished();

private:
    void initUI();

    void createLeverArmBox(QFormLayout * leverArmLayout);

    QGroupBox * leverArmBox;

    QLineEdit * xLeverArm;
    QLineEdit * yLeverArm;
    QLineEdit * zLeverArm;

    double offsetX;
    double offsetY;
    double offsetZ;

    Project *   project            = NULL;
};

#endif // VESSELGEOMETRYWINDOW_H
