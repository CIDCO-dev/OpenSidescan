#ifndef IMAGETAB_H
#define IMAGETAB_H

#include <QWidget>
#include <QScrollArea>
#include <QLabel>
#include <QPixmap>
#include <QRubberBand>
#include <QPoint>
#include <QMouseEvent>
#include <QCheckBox>

#include "sidescan/sidescanimage.h"
#include "sidescan/sidescanfile.h"
#include "imagetablabel.h"
#include "ui/mainwindow/mainwindow.h"

class ImageTabLabel;

class ImageTab : public QWidget
{
    Q_OBJECT

private:
    SidescanImage & image;

    QAction * showObjectBoundingBox;
    QAction * showObjectSizesAction;
    QAction * showObjectCenter;
    QAction * showMicroFeatures;


    ImageTabLabel * imageLabel;
    QScrollArea * scrollArea;

public:
    ImageTab(SidescanImage & image,QWidget *parent);

    SidescanImage * getImage(){ return &image;}
    QScrollArea & getScrollArea() { return *scrollArea;}

signals:
    void inventoryChanged();

public slots:
    void saveImage();
    void refreshImage();
    void refreshInventory();
};

#endif // IMAGETAB_H
