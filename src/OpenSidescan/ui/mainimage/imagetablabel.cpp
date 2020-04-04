#include "imagetablabel.h"

#include <algorithm>
#include <iostream>

#include "inventoryobject/inventoryobject.h"
#include "ui/mainwindow/mainwindow.h"
#include "ui/inventory/inventoryobjectwindow.h"
#include "ui/inventory/inventoryobjectmenu.h"

ImageTabLabel::ImageTabLabel(ImageTab & parent, SidescanFile & file,SidescanImage & img) : parent(parent),file(file),img(img)
{
    rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
}


/* FIXME: Complete ROI selection */

GeoreferencedObject * ImageTabLabel::insideObject(QPoint & p){
    for(auto i=img.getObjects().begin();i!=img.getObjects().end();i++){
        if(
                (p.x() >= (*i)->getX() && p.x() < (*i)->getX() + (*i)->getPixelWidth()) &&
                (p.y() >= (*i)->getY() && p.y() < (*i)->getY() + (*i)->getPixelHeight())
        ){
            return *i;
        }
    }

    return NULL;
}

void ImageTabLabel::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        // Left button starts a rubberband to select a region
        selectionOrigin = event->pos();

       //TODO: determine if we clicked inside an object's bounding box and popup the object details dialog if so

        if(GeoreferencedObject * obj = insideObject(selectionOrigin)){
            GeoreferencedObjectWindow objectWindow(obj);

            int res = objectWindow.exec();

            if(res == QDialog::Accepted){
                emit inventoryChanged();
            }
        }
        else{
            rubberBand->setGeometry(QRect(selectionOrigin, QSize()));
            rubberBand->show();
        }
    }
    else if(event->button() == Qt::RightButton){
        QPoint clickPosition = event->pos();

        if(rubberBand->isVisible()){
            //right button while dragging cancels current selection
            rubberBand->hide();
        }
        else if(GeoreferencedObject * obj = insideObject(clickPosition)){
            GeoreferencedObjectMenu mnu(obj);
            connect(&mnu,&GeoreferencedObjectMenu::inventoryChanged,this,&ImageTabLabel::emitInventoryChanged);

            mnu.exec(QCursor::pos());
        }
    }
}

void ImageTabLabel::emitInventoryChanged(){
    //indirection for sub modules to reach the top module
    emit inventoryChanged();
}


void ImageTabLabel::mouseMoveEvent(QMouseEvent *event)
{
    rubberBand->setGeometry(QRect(selectionOrigin, event->pos()).normalized());
}

void ImageTabLabel::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){

        if(rubberBand->isVisible()){
            QPoint p = event->pos();

            rubberBand->hide();

            //upper left
            int x1 = (std::min)( (std::max)( (std::min)( selectionOrigin.x(), p.x() ) , 0 ) , img.getImage().cols - 1 );
            int y1 = (std::min)( (std::max)( (std::min)( selectionOrigin.y(),p.y() ) , 0 ) , img.getImage().rows - 1 );

            //lower right
            int x2 = (std::min)( (std::max)( (std::max)( selectionOrigin.x(),p.x() ), 0 ) , img.getImage().cols - 1 );
            int y2 = (std::min)( (std::max)( (std::max)( selectionOrigin.y(),p.y() ), 0 ) , img.getImage().rows - 1 );

            int width  = x2-x1;
            int height = y2-y1;

            if(width > 0 && height > 0){
                GeoreferencedObject*  obj = new GeoreferencedObject(file,img,x1,y1,width, height);
                img.getObjects().push_back(obj);

                parent.refreshImage();

                GeoreferencedObjectWindow newObjectWindow(obj);

                if ( newObjectWindow.exec() ) {
                    emit inventoryChanged();
                } else {
                    // User pressed Cancel on the dialog, don't add the object to the inventory

                    img.getObjects().pop_back();

                    delete obj;

                    parent.refreshImage();
                }

            }

//            std::cout << "\nImageTabLabel::mouseReleaseEvent(): width: " << width << ", height: " << height << "\n" << std::endl;


            // determine selection, for example using QRect::intersects()
            // and QRect::contains().
        }
    }
}
