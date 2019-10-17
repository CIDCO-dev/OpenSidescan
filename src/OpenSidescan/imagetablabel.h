#ifndef IMAGETABLABEL_H
#define IMAGETABLABEL_H

#include <QLabel>
#include <QRubberBand>
#include <QPoint>
#include <QMouseEvent>

#include "sidescanfile.h"
#include "sidescanimage.h"

#include "imagetab.h"

class ImageTab;

class ImageTabLabel : public QLabel
{
public:
    ImageTabLabel(ImageTab & parent,SidescanFile & file,SidescanImage & img);

protected:

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    ImageTab & parent;

    SidescanFile & file;
    SidescanImage & img;

    QPoint selectionOrigin;
    QRubberBand * rubberBand = NULL;

    Q_OBJECT
};

#endif // IMAGETABLABEL_H
