#include "ui/docks/vesselgeometrywindow.h"

VesselGeometryWindow::VesselGeometryWindow(QWidget * parent) :  QDockWidget(tr("Vessel Geometry"),parent),project(NULL)
{
    initUI();
}

 void VesselGeometryWindow::setProject(Project * p){
     project = p;

     if(p){
         //load from project
         offsetX = p->getAntenna2TowPointLeverArm()(0);
         xLeverArm->setText(QString::number(offsetX,'f',3));
         xLeverArm->setEnabled(true);

         offsetY = p->getAntenna2TowPointLeverArm()(1);
         yLeverArm->setText(QString::number(offsetY,'f',3));
         yLeverArm->setEnabled(true);

         offsetZ = p->getAntenna2TowPointLeverArm()(2);
         zLeverArm->setText(QString::number(offsetZ,'f',3));
         zLeverArm->setEnabled(true);
     }
     else{
         //disable all
         offsetX=offsetY=offsetZ=0;

         xLeverArm->clear();
         yLeverArm->clear();
         zLeverArm->clear();
         xLeverArm->setEnabled(false);
         yLeverArm->setEnabled(false);
         zLeverArm->setEnabled(false);
     }
 }

void VesselGeometryWindow::editingFinished() {
    /*QDoubleValidator validator(-1000000.0, 1000000.0, 4);

    QString x = xLeverArm->text();
    QString y = yLeverArm->text();
    QString z = zLeverArm->text();
    int pos = 0;
    if(validator.validate(x,pos) == QDoubleValidator::Invalid) {
        xLeverArm->clear();
        return;
    }

    if(validator.validate(y,pos) == QDoubleValidator::Invalid) {
        yLeverArm->clear();
        return;
    }

    if(validator.validate(z,pos) == QDoubleValidator::Invalid) {
        zLeverArm->clear();
        return;
    }*/

    offsetX = xLeverArm->text().toDouble();
    offsetY = yLeverArm->text().toDouble();
    offsetZ = zLeverArm->text().toDouble();

    project->setAntenna2TowPointLeverArm(offsetX,offsetY,offsetZ);
}

void VesselGeometryWindow::initUI() {
    QWidget * mainWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainWidget->setLayout(mainLayout);

    //lever arm parameters box

    leverArmBox = new QGroupBox(tr("GPS to Tow Point Lever Arm"));

    QFormLayout * leverArmLayout = new QFormLayout();
    leverArmBox->setLayout(leverArmLayout);

    createLeverArmBox(leverArmLayout);

    mainLayout->addWidget(leverArmBox);

    this->setWidget(mainWidget);
    this->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
    this->setFeatures(QDockWidget::DockWidgetClosable|QDockWidget::DockWidgetMovable|QDockWidget::DockWidgetFloatable);
    this->show();
}

void VesselGeometryWindow::createLeverArmBox(QFormLayout * leverArmLayout) {
    xLeverArm = new QLineEdit();
    xLeverArm->setValidator( new QDoubleValidator(-1000000.0, 1000000.0, 4, this) );
    xLeverArm->setAlignment(Qt::AlignRight);
    leverArmLayout->addRow(new QLabel(tr("X :")),xLeverArm);
    connect(xLeverArm, &QLineEdit::editingFinished, this, &VesselGeometryWindow::editingFinished);

    yLeverArm = new QLineEdit();
    yLeverArm->setValidator( new QDoubleValidator(-1000000.0, 1000000.0, 4, this) );
    yLeverArm->setAlignment(Qt::AlignRight);
    yLeverArm->setText(QString::number(offsetY,'f',3));
    leverArmLayout->addRow(new QLabel(tr("Y :")),yLeverArm);
    connect(yLeverArm, &QLineEdit::editingFinished, this, &VesselGeometryWindow::editingFinished);

    zLeverArm = new QLineEdit();
    zLeverArm->setValidator( new QDoubleValidator(-1000000.0, 1000000.0, 4, this) );
    zLeverArm->setAlignment(Qt::AlignRight);
    zLeverArm->setText(QString::number(offsetZ,'f',3));
    leverArmLayout->addRow(new QLabel(tr("Z :")),zLeverArm);
    connect(zLeverArm, &QLineEdit::editingFinished, this, &VesselGeometryWindow::editingFinished);
}
