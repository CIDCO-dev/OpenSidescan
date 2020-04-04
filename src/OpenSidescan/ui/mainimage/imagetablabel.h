#ifndef IMAGETABLABEL_H
#define IMAGETABLABEL_H

#include <QLabel>
#include <QRubberBand>
#include <QPoint>
#include <QMouseEvent>

#include "sidescan/sidescanfile.h"
#include "sidescan/sidescanimage.h"

#include "imagetab.h"

class ImageTab;

class ImageTabLabel : public QLabel
{    
    Q_OBJECT

public:
    ImageTabLabel(ImageTab & parent,SidescanFile & file,SidescanImage & img);

    GeoreferencedObject * insideObject(QPoint & p);

protected:

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

signals:
    void inventoryChanged();

public slots:
    void emitInventoryChanged();

private:
    ImageTab & parent;

    SidescanFile & file;
    SidescanImage & img;

    QPoint selectionOrigin;
    QRubberBand * rubberBand = NULL;
};

#endif // IMAGETABLABEL_H
