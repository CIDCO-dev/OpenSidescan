#include "filepropertieswindow.h"

#include <QHeaderView>

FilePropertiesWindow::FilePropertiesWindow(QWidget *parent) : QDockWidget(tr("File properties"),parent)
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

void FilePropertiesWindow::setFile(SidescanFile * file){
    updateModel(file);
}

void FilePropertiesWindow::updateModel(SidescanFile * file){
    propertiesTable->clear();

    if(file) {

        propertiesTable->setColumnCount(2);
        propertiesTable->setRowCount(file->getFileProperties().size()+1);

        QStringList headers;
        headers << "Property" << "Value";
        propertiesTable->setHorizontalHeaderLabels(headers);

        int row = 0;

        for(
            auto i = file->getFileProperties().begin();
            i != file->getFileProperties().end();
            i++
        ){
            propertiesTable->setItem(row,0,new QTableWidgetItem(QString::fromStdString(i->first)));
            propertiesTable->setItem(row,1,new QTableWidgetItem(QString::fromStdString(i->second)));

            row++;
        }

        propertiesTable->setItem(row,0,new QTableWidgetItem(QString::fromStdString("Average X Distance Per Pixel")));
        propertiesTable->setItem(row,1,new QTableWidgetItem(QString::fromStdString( std::to_string( file->getAverageXDistancePerPixel() ))));

    }
    else {
        propertiesTable->setColumnCount(2);
        QStringList headers;
        headers << "Property" << "Value";
        propertiesTable->setHorizontalHeaderLabels(headers);
    }

}
