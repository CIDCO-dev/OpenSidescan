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

    friend class testGUI;

private:
    QTableWidget * inventoryTable;

    Project * project;


public:
    InventoryWindow(QWidget * parent);

    void setProject(Project * p);

signals:
    void objectSelected(GeoreferencedObject * object);

public slots:
    void refreshInventoryTable();
    void doubleClicked(const QModelIndex & index);
};

#endif // INVENTORYWINDOW_H
