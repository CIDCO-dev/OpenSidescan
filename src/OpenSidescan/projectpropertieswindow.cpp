#include "projectpropertieswindow.h"
#include "mainwindow.h"

ProjectPropertiesWindow::ProjectPropertiesWindow(QWidget * parent) : QDialog(parent), project(((MainWindow*)parent)->getProject())
{
    initUI();
}

void ProjectPropertiesWindow::ok() {
    QDoubleValidator validator(-1000000.0, 1000000.0, 4);

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
    }


    Eigen::Vector3d leverArm(xLeverArm->text().toDouble(), yLeverArm->text().toDouble(), zLeverArm->text().toDouble());
    project->setAntenna2TowPointLeverArm(leverArm);
    this->accept();
}

void ProjectPropertiesWindow::cancel() {
    this->reject();
}

void ProjectPropertiesWindow::initUI() {
    this->setWindowTitle("Project Properties");

    QVBoxLayout *mainLayout = new QVBoxLayout();

    //lever arm parameters box

    leverArmBox = new QGroupBox(tr("GPS to Tow Point Lever Arm"));

    QFormLayout * leverArmLayout = new QFormLayout();
    leverArmBox->setLayout(leverArmLayout);

    createLeverArmBox(leverArmLayout);

    mainLayout->addWidget(leverArmBox);

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    buttonBox->setObjectName( "buttonBox" );

    connect(buttonBox, &QDialogButtonBox::accepted, this, &ProjectPropertiesWindow::ok);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &ProjectPropertiesWindow::cancel);

    mainLayout->addWidget(buttonBox);

    this->setLayout(mainLayout);
}

void ProjectPropertiesWindow::createLeverArmBox(QFormLayout * leverArmLayout) {
    project->getFiles();

    Eigen::Vector3d leverArm = project->getAntenna2TowPointLeverArm();

    xLeverArm = new QLineEdit();
    xLeverArm->setValidator( new QDoubleValidator(-1000000.0, 1000000.0, 4, this) );
    xLeverArm->setAlignment(Qt::AlignRight);
    xLeverArm->setText(QString::number(leverArm[0],'f',3));
    leverArmLayout->addRow(new QLabel(tr("X :")),xLeverArm);

    yLeverArm = new QLineEdit();
    yLeverArm->setValidator( new QDoubleValidator(-1000000.0, 1000000.0, 4, this) );
    yLeverArm->setAlignment(Qt::AlignRight);
    yLeverArm->setText(QString::number(leverArm[1],'f',3));
    leverArmLayout->addRow(new QLabel(tr("Y :")),yLeverArm);

    zLeverArm = new QLineEdit();
    zLeverArm->setValidator( new QDoubleValidator(-1000000.0, 1000000.0, 4, this) );
    zLeverArm->setAlignment(Qt::AlignRight);
    zLeverArm->setText(QString::number(leverArm[2],'f',3));
    leverArmLayout->addRow(new QLabel(tr("Z :")),zLeverArm);
}
