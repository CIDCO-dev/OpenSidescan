#include "inventoryobjectwindow.h"

#include <QLabel>
#include <QTextEdit>
#include <QLineEdit>
#include <QDialogButtonBox>

#include "../thirdParty/MBES-lib/src/utils/StringUtils.hpp"

InventoryObjectWindow::InventoryObjectWindow(InventoryObject * obj):object(obj)
{
    initUI();
}

void InventoryObjectWindow::initUI(){
    QVBoxLayout * container = new QVBoxLayout();

    QFormLayout * layout = new QFormLayout();

    QLabel * lblName = new QLabel(tr("Name"),this);
    txtName = new QLineEdit(QString::fromStdString(object->getName()),this);

    layout->addRow(lblName,txtName);

    QLabel * lblChannel = new QLabel(tr("Channel"),this);
    QLabel * txtChannel = new QLabel(tr(object->getImage().getChannelName().c_str()),this);

    layout->addRow(lblChannel,txtChannel);

    QLabel * lblLatitude  = new QLabel(tr("Latitude"),this);
    QLabel * txtLatitude  = new QLabel(tr((object->getPosition()!=NULL)?StringUtils::to_string_with_precision(object->getPosition()->getLatitude(),12).c_str():""),this);

    layout->addRow(lblLatitude,txtLatitude);

    QLabel * lblLongitude  = new QLabel(tr("Longitude"),this);
    QLabel * txtLongitude  = new QLabel(tr((object->getPosition()!=NULL)?StringUtils::to_string_with_precision(object->getPosition()->getLongitude(),12).c_str():""),this);

    layout->addRow(lblLongitude,txtLongitude);


    QLabel * lblWidth  = new QLabel(tr("Width"),this);
    QLabel * txtWidth  = new QLabel(tr(std::to_string(object->getWidth()).c_str()),this);

    layout->addRow(lblWidth,txtWidth);

    QLabel * lblHeight  = new QLabel(tr("Height"),this);

    std::string heightString = (!std::isnan(object->getHeight())) ?std::to_string(object->getHeight()) : "Unknown" ;
    QLabel * txtHeight  = new QLabel(tr(heightString.c_str()),this);

    layout->addRow(lblHeight,txtHeight);

    QLabel * lblDescription = new QLabel(tr("Description"),this);
    txtDescription = new QTextEdit(QString::fromStdString(object->getDescription()),this);

    layout->addRow(lblDescription,txtDescription);
    container->addLayout(layout);

    //buttons
    QDialogButtonBox * buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    buttonBox->setObjectName( "buttonBox" );

    connect(buttonBox, &QDialogButtonBox::accepted, this, &InventoryObjectWindow::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &InventoryObjectWindow::close);

    container->addWidget(buttonBox);

    this->setLayout(container);

    this->setWindowTitle("Object Information");
    this->setModal(true);
    this->setVisible(true);

}

void InventoryObjectWindow::accept(){
    //TODO: save values
    std::string name = txtName->text().toStdString();
    std::string description = txtDescription->toPlainText().toStdString();

    object->setName(name);
    object->setDescription(description);

    QDialog::accept();
}

