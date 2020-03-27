#include "projectpropertieswindow.h"

ProjectPropertiesWindow::ProjectPropertiesWindow(QWidget * parent) : QDialog(parent)
{
    initUI();
}

void ProjectPropertiesWindow::ok() {
    Eigen::Vector3d leverArm(xLeverArm->text().toDouble(), yLeverArm->text().toDouble(), zLeverArm->text().toDouble());
    project->setAntenna2TowPointLeverArm(leverArm);
}

void ProjectPropertiesWindow::cancel() {
    //do nothing
}

void ProjectPropertiesWindow::initUI() {
    this->setWindowTitle("Find Objects");

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
}

void ProjectPropertiesWindow::createLeverArmBox(QFormLayout * leverArmLayout) {
    xLeverArm = new QLineEdit();
    xLeverArm->setValidator( new QDoubleValidator(-1000000.0, 1000000.0, 4, this) );
    xLeverArm->setAlignment(Qt::AlignRight);
    xLeverArm->setText(QString::fromStdString(std::to_string(project->getAntenna2TowPointLeverArm()[0])));
    leverArmLayout->addRow(new QLabel(tr("X :")),xLeverArm);

    yLeverArm = new QLineEdit();
    yLeverArm->setValidator( new QDoubleValidator(-1000000.0, 1000000.0, 4, this) );
    yLeverArm->setAlignment(Qt::AlignRight);
    yLeverArm->setText(QString::fromStdString(std::to_string(project->getAntenna2TowPointLeverArm()[0])));
    leverArmLayout->addRow(new QLabel(tr("X :")),yLeverArm);

    zLeverArm = new QLineEdit();
    zLeverArm->setValidator( new QDoubleValidator(-1000000.0, 1000000.0, 4, this) );
    zLeverArm->setAlignment(Qt::AlignRight);
    zLeverArm->setText(QString::fromStdString(std::to_string(project->getAntenna2TowPointLeverArm()[0])));
    leverArmLayout->addRow(new QLabel(tr("X :")),zLeverArm);
}
