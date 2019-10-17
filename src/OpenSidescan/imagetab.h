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

#include "sidescanimage.h"
#include "sidescanfile.h"
#include "imagetablabel.h"
#include "mainwindow.h"

class ImageTabLabel;

class ImageTab : public QWidget
{
private:
    SidescanImage & image;
    SidescanFile & file;

    QAction * showObjectBoundingBox;
    QAction * showObjectSizesAction;
    QAction * showObjectCenter;


    ImageTabLabel * imageLabel;
    QScrollArea * scrollArea;


    Q_OBJECT
public:
    explicit ImageTab(SidescanFile & file,SidescanImage & image,QWidget *parent);

    MainWindow * getParent() { return (MainWindow*) this->parent();}

signals:

public slots:
    void saveImage();
    void refreshImage();

};

#endif // IMAGETAB_H
