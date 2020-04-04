#ifndef INVENTORYOBJECTWINDOW_H
#define INVENTORYOBJECTWINDOW_H

#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QTextEdit>

#include "inventoryobject/inventoryobject.h"

class InventoryObjectWindow : public QDialog
{
public:
    InventoryObjectWindow(InventoryObject * obj);

    InventoryObject & getObject();

public slots:
    void accept();

private:
    void initUI();

    InventoryObject * object;
    QLineEdit * txtName;
    QTextEdit * txtDescription;

    Q_OBJECT;
};

#endif // INVENTORYOBJECTWINDOW_H
