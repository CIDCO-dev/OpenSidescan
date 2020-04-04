#ifndef GEOREFERENCEDOBJECTWINDOW_H
#define GEOREFERENCEDOBJECTWINDOW_H

#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QTextEdit>

#include "inventoryobject/inventoryobject.h"

class GeoreferencedObjectWindow : public QDialog
{
public:
    GeoreferencedObjectWindow(GeoreferencedObject * obj);

    GeoreferencedObject & getObject();

public slots:
    void accept();

private:
    void initUI();

    GeoreferencedObject * object;
    QLineEdit * txtName;
    QTextEdit * txtDescription;

    Q_OBJECT;
};

#endif // GEOREFERENCEDOBJECTWINDOW_H
