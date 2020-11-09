#include "inventorywindow.h"

#include <sstream>
#include "ui/mainwindow/mainwindow.h"
#include <QFileInfo>
#include "ui/inventory/inventoryobjectmenu.h"

class MainWindow;

class InventoryTableItem : public QTableWidgetItem{
public:
    InventoryTableItem(QString text) : QTableWidgetItem(text), object(NULL){

    }

    ~InventoryTableItem(){

    }

    InventoryObject * object;
};

InventoryWindow::InventoryWindow(QWidget * parent): QDockWidget(tr("Object Inventory"),parent),project(NULL)
{
    inventoryTable = new QTableWidget(this);

    inventoryTable->verticalHeader()->setVisible(false);
    inventoryTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    inventoryTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    inventoryTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    inventoryTable->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(inventoryTable, SIGNAL(customContextMenuRequested(QPoint)), SLOT(georefObjectMenuRequested(QPoint)));

    connect(inventoryTable,&QTableWidget::doubleClicked,this,&InventoryWindow::doubleClicked);

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

void InventoryWindow::doubleClicked(const QModelIndex & index){
    InventoryTableItem * item = (InventoryTableItem*) inventoryTable->item(index.row(),0);

    if(item && item->object){
        emit objectSelected(item->object);
    }
}

void InventoryWindow::georefObjectMenuRequested(QPoint pos) {
    QModelIndex index = inventoryTable->indexAt(pos);
    InventoryTableItem * item = (InventoryTableItem*) inventoryTable->item(index.row(),0);

    if(item && item->object) {
        InventoryObjectMenu mnu(item->object);
        connect(&mnu,&InventoryObjectMenu::inventoryChanged,this,&InventoryWindow::refreshInventoryTable);
        connect(&mnu,&InventoryObjectMenu::inventoryChanged,(MainWindow*)this->parent(),&MainWindow::refreshTabs);
        mnu.exec(inventoryTable->viewport()->mapToGlobal(pos));
    }
}

void InventoryWindow::addObjects(SidescanFile & file){
    std::stringstream ss;
    int row = this->inventoryTable->rowCount();

        for(auto j=file.getImages().begin();j!=file.getImages().end();j++){
            for(auto k=(*j)->getObjects().begin();k!=(*j)->getObjects().end();k++){
                this->inventoryTable->setRowCount(row+1);

                InventoryTableItem * rowFirst = new InventoryTableItem(QString::fromStdString((*k)->getName()));

                rowFirst->object = *k;
                rowFirst->setTextAlignment(Qt::AlignHCenter);

                this->inventoryTable->setItem(row,0,rowFirst);

                QFileInfo fileInfo( QString::fromStdString(file.getFilename()) );

                QString filenameWithoutPath = fileInfo.fileName();

                QTableWidgetItem * item = new QTableWidgetItem( filenameWithoutPath );

                item->setTextAlignment(Qt::AlignHCenter);

                this->inventoryTable->setItem(row,1,item);

                item = new QTableWidgetItem(QString::fromStdString((*j)->getChannelName()));
                item->setTextAlignment(Qt::AlignHCenter);

                this->inventoryTable->setItem(row,2,item);

                ss << std::setprecision(15);

                Position * pos = (*k)->getPosition();

                if(pos){
                    ss << pos->getLongitude();
                }
                else{
                    ss << "N/A";
                }

                item = new QTableWidgetItem(QString::fromStdString(ss.str()));
                item->setTextAlignment(Qt::AlignHCenter);

                this->inventoryTable->setItem(row,3,item);

                ss.str(std::string());

                if(pos){
                    ss << pos->getLatitude();
                }
                else{
                    ss << "N/A";
                }

                item = new QTableWidgetItem(QString::fromStdString(ss.str()));
                item->setTextAlignment(Qt::AlignHCenter);

                this->inventoryTable->setItem(row,4,item);

                ss << std::setprecision(3);

                ss.str(std::string());

                if((*k)->getWidth() > 0){
                    ss << (*k)->getWidth() ;
                }
                else{
                    ss << "N/A";
                }

                item = new QTableWidgetItem(QString::fromStdString(ss.str()));
                item->setTextAlignment(Qt::AlignHCenter);

                this->inventoryTable->setItem(row,5,item);

                ss.str(std::string());

                if((*k)->getHeight() > 0){
                    ss << (*k)->getHeight() ;
                }
                else{
                    ss << "N/A";
                }

                item = new QTableWidgetItem(QString::fromStdString(ss.str()));
                item->setTextAlignment(Qt::AlignHCenter);

                this->inventoryTable->setItem(row,6,item);

                ss.str(std::string());

                row++;
            }
        }

}

void InventoryWindow::refreshInventoryTable(){

    this->inventoryTable->clear();
    this->inventoryTable->setRowCount(0);

    this->inventoryTable->setColumnCount(7);

    QStringList headers;
    headers << "Name" << "File" << "Channel" << "Longitude" << "Latitude" << "Width (m)" << "Length (m)";
    this->inventoryTable->setHorizontalHeaderLabels(headers);

    int row =0;

    if(project){
        project->walkFiles(this,&InventoryWindow::addObjects);




    }
}

