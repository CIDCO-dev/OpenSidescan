#ifndef INVENTORYOBJECTMENU_H
#define INVENTORYOBJECTMENU_H

#include <QMenu>
#include "inventoryobject/inventoryobject.h"

class InventoryObjectMenu : public QMenu
{

    Q_OBJECT

public:
    InventoryObjectMenu(InventoryObject * obj);
    ~InventoryObjectMenu();

    void initUI();

signals:
    void inventoryChanged();

public slots:
    void viewObjectDetails();
    void deleteObject();

private:
    InventoryObject * object=NULL;

};

#endif // INVENTORYOBJECTMENU_H
