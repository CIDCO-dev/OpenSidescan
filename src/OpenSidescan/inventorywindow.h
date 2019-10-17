#ifndef INVENTORYWINDOW_H
#define INVENTORYWINDOW_H

#include <QDockWidget>
#include <QTableWidget>
#include <QHeaderView>

#include "georeferencedobject.h"
#include "project.h"

class InventoryWindow : public QDockWidget
{
    Q_OBJECT

private:
    QTableWidget * inventoryTable;

    Project * project;



public:
    InventoryWindow(QWidget * parent);

    void setProject(Project * p);
    void refreshInventoryTable();

public slots:

};

#endif // INVENTORYWINDOW_H
