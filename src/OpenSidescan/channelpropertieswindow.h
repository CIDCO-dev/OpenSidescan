#ifndef CHANNELPROPERTIESWINDOW_H
#define CHANNELPROPERTIESWINDOW_H

#include <QDockWidget>
#include <QTableWidget>
#include "sidescanimage.h"

class ChannelPropertiesWindow : public QDockWidget
{
    Q_OBJECT

    friend class testGUI;

public:
    ChannelPropertiesWindow(QWidget *parent);

    void updateModel(SidescanImage * image);

    void initTableHeaders();

private:
    QTableWidget * propertiesTable;
};

#endif // CHANNELPROPERTIESWINDOW_H
