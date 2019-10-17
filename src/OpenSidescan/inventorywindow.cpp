#include "inventorywindow.h"

#include <sstream>

InventoryWindow::InventoryWindow(QWidget * parent): QDockWidget(tr("Object Inventory"),parent),project(NULL)
{
    inventoryTable = new QTableWidget(this);

    inventoryTable->verticalHeader()->setVisible(false);
    inventoryTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    inventoryTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    inventoryTable->setSelectionBehavior(QAbstractItemView::SelectRows);

    this->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea|Qt::BottomDockWidgetArea);
    this->setWidget(inventoryTable);
    this->setFeatures(QDockWidget::DockWidgetClosable|QDockWidget::DockWidgetMovable|QDockWidget::DockWidgetFloatable);
    this->show();

    refreshInventoryTable();
}

void InventoryWindow::setProject(Project * p){
    project = p;
    refreshInventoryTable();
}

void InventoryWindow::refreshInventoryTable(){

    std::cout << this->inventoryTable << std::endl;


    this->inventoryTable->clear();

    this->inventoryTable->setColumnCount(6);


    QStringList headers;
    headers << "File" << "Channel" << "Longitude" << "Latitude" << "Width (m)" << "Height (m)";
    this->inventoryTable->setHorizontalHeaderLabels(headers);

    int row =0;

    if(project){
        std::stringstream ss;

        ss << std::setprecision(15);

        for(auto i=project->getFiles().begin();i!=project->getFiles().end();i++){
            for(auto j=(*i)->getImages().begin();j!=(*i)->getImages().end();j++){
                for(auto k=(*j)->getObjects().begin();k!=(*j)->getObjects().end();k++){

                    this->inventoryTable->setRowCount(row+1);

                    this->inventoryTable->setItem(row,0,new QTableWidgetItem(QString::fromStdString((*i)->getFilename())));

                    this->inventoryTable->setItem(row,1,new QTableWidgetItem(QString::fromStdString((*j)->getChannelName())));

                    Position * pos = (*k)->getPosition();

                    if(pos){
                        ss << pos->getLongitude();
                    }
                    else{
                        ss << "N/A";
                    }

                    this->inventoryTable->setItem(row,2,new QTableWidgetItem(QString::fromStdString(ss.str())));

                    ss.str(std::string());

                    if(pos){
                        ss << pos->getLatitude();
                    }
                    else{
                        ss << "N/A";
                    }

                    this->inventoryTable->setItem(row,3,new QTableWidgetItem(QString::fromStdString(ss.str())));

                    ss.str(std::string());

                    if((*k)->getWidth() > 0){
                        ss << (*k)->getWidth() ;
                    }
                    else{
                        ss << "N/A";
                    }

                    this->inventoryTable->setItem(row,4,new QTableWidgetItem(QString::fromStdString(ss.str())));

                    ss.str(std::string());

                    if((*k)->getHeight() > 0){
                        ss << (*k)->getHeight() ;
                    }
                    else{
                        ss << "N/A";
                    }

                    this->inventoryTable->setItem(row,5,new QTableWidgetItem(QString::fromStdString(ss.str())));

                    ss.str(std::string());

                    row++;
                }
            }
        }
    }
}
