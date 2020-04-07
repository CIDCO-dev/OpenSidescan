#include "inventoryobjectmenu.h"
#include "inventoryobjectwindow.h"

#include <QMessageBox>

InventoryObjectMenu::InventoryObjectMenu(InventoryObject * obj) : object(obj)
{
    initUI();
}

InventoryObjectMenu::~InventoryObjectMenu(){

}

void InventoryObjectMenu::initUI(){
    this->setTitle(tr("Object Menu"));

    QAction * actionView = new QAction(QIcon(":/Images/resources/view.png"),tr("View object details"));
    connect(actionView,&QAction::triggered,this,&InventoryObjectMenu::viewObjectDetails);
    this->addAction(actionView);

    QAction * actionDelete = new QAction(QIcon(":/Images/resources/delete.png"),tr("Remove from inventory"));
    connect(actionDelete,&QAction::triggered,this,&InventoryObjectMenu::deleteObject);
    this->addAction(actionDelete);
}

void InventoryObjectMenu::viewObjectDetails(){
    InventoryObjectWindow win(object);

    int res = win.exec();

    if(res == QDialog::Accepted){
        emit inventoryChanged();
    }
}

void InventoryObjectMenu::deleteObject(){

    QMessageBox msgBox;
    msgBox.setText("This will permanently remove the object from your inventory. Are you sure you want to proceed?");
    msgBox.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
    msgBox.setDefaultButton(QMessageBox::No);
    int ret = msgBox.exec();

    if(ret == QMessageBox::Yes){
        SidescanImage & img = object->getImage();

        //TODO: use a list instead
        std::vector<InventoryObject*> & objects = img.getObjects();
        for(auto i = objects.begin();i!=objects.end();i++){
            if(*i==object){
                objects.erase(i);
                delete object;
                object=NULL;
                emit inventoryChanged();
                break;
            }
        }
    }

}
