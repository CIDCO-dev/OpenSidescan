#include "imagetablabel.h"

#include <iostream>
#include "georeferencedobject.h"
#include "mainwindow.h"

ImageTabLabel::ImageTabLabel(ImageTab & parent, SidescanFile & file,SidescanImage & img) : parent(parent),file(file),img(img)
{
    //TODO: cleanup rubberband
}


/* FIXME: Complete ROI selection */

void ImageTabLabel::mousePressEvent(QMouseEvent *event)
{
    selectionOrigin = event->pos();


   //TODO: determine if we clicked inside an object's bounding box and popup the object details dialog if so
    bool insideObject = false;

    if(insideObject){
        std::cout << "DONG" << std::endl;
    }
    else{
        //Else;
        if (!rubberBand)
            rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
        rubberBand->setGeometry(QRect(selectionOrigin, QSize()));
        rubberBand->show();

        std::cout << "Ding " << selectionOrigin.x() << "," << selectionOrigin.y() << std::endl;
    }
}


void ImageTabLabel::mouseMoveEvent(QMouseEvent *event)
{
    rubberBand->setGeometry(QRect(selectionOrigin, event->pos()).normalized());
}

void ImageTabLabel::mouseReleaseEvent(QMouseEvent *event)
{

    QPoint p = event->pos();

    GeoreferencedObject*  obj = new GeoreferencedObject(file,img,selectionOrigin.x(),selectionOrigin.y(),p.x() - selectionOrigin.x(), p.y() - selectionOrigin.y());


    img.getObjects().push_back(obj);

    rubberBand->hide();

    parent.refreshImage();
    parent.getParent()->refreshObjectInventory();

    // determine selection, for example using QRect::intersects()
    // and QRect::contains().

}
