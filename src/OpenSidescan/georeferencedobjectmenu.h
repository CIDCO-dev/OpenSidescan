#ifndef GEOREFERENCEDOBJECTMENU_H
#define GEOREFERENCEDOBJECTMENU_H

#include <QMenu>
#include "georeferencedobject.h"

class GeoreferencedObjectMenu : public QMenu
{

    Q_OBJECT

public:
    GeoreferencedObjectMenu(GeoreferencedObject * obj);
    ~GeoreferencedObjectMenu();

    void initUI();

signals:
    void inventoryChanged();

public slots:
    void viewObjectDetails();
    void deleteObject();

private:
    GeoreferencedObject * object=NULL;

};

#endif // GEOREFERENCEDOBJECTMENU_H
