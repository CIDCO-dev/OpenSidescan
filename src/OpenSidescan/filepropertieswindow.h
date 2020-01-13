#ifndef FILEPROPERTIESWINDOW_H
#define FILEPROPERTIESWINDOW_H

#include <QDockWidget>
#include <QTableWidget>
#include "sidescanfile.h"

class FilePropertiesWindow : public QDockWidget
{
    Q_OBJECT

    friend class testGUI;

public:
    FilePropertiesWindow(QWidget *parent);

    void setFile(SidescanFile * file);

    void updateModel(SidescanFile * file);

private:
    QTableWidget * propertiesTable;
};

#endif // FILEPROPERTIESWINDOW_H
