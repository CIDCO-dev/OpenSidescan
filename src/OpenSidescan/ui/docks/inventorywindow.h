#ifndef INVENTORYWINDOW_H
#define INVENTORYWINDOW_H

#include <QDockWidget>
#include <QTableWidget>
#include <QHeaderView>

#include "inventoryobject/inventoryobject.h"
#include "project/project.h"

class InventoryWindow : public QDockWidget
{
    Q_OBJECT

    friend class testGUI;

private:
    QTableWidget * inventoryTable;

    Project * project;

protected:

public:
    InventoryWindow(QWidget * parent);

    void setProject(Project * p);

signals:
    void objectSelected(GeoreferencedObject * object);

public slots:
    void refreshInventoryTable();
    void doubleClicked(const QModelIndex & index);
    void georefObjectMenuRequested(QPoint pos);
};

#endif // INVENTORYWINDOW_H
