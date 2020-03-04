#include "channelpropertieswindow.h"

#include <QHeaderView>

ChannelPropertiesWindow::ChannelPropertiesWindow(QWidget *parent) : QDockWidget(tr("Channel properties"),parent)
{
    propertiesTable = new QTableWidget(this);

    propertiesTable->verticalHeader()->setVisible(false);
    propertiesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    propertiesTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    propertiesTable->setSelectionBehavior(QAbstractItemView::SelectRows);

    this->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
    this->setWidget(propertiesTable);
    this->setFeatures(QDockWidget::DockWidgetClosable|QDockWidget::DockWidgetMovable|QDockWidget::DockWidgetFloatable);
    this->show();
}

void ChannelPropertiesWindow::initTableHeaders(){
    propertiesTable->setColumnCount(2);
    QStringList headers;
    headers << "Property" << "Value";
    propertiesTable->setHorizontalHeaderLabels(headers);
}

void ChannelPropertiesWindow::updateModel(SidescanImage * image){
    propertiesTable->clear();

    initTableHeaders();

    if(image) {
        propertiesTable->setRowCount(image->getProperties().size()+1);

        int row = 0;

        for(
            auto i = image->getProperties().begin();
            i != image->getProperties().end();
            i++
        ){
            propertiesTable->setItem(row,0,new QTableWidgetItem(QString::fromStdString(i->first)));
            propertiesTable->setItem(row,1,new QTableWidgetItem(QString::fromStdString(i->second)));

            row++;
        }
    }
    else {
        propertiesTable->setRowCount(0);
    }

}
