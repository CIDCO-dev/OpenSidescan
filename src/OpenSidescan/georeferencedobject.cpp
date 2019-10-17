#include "georeferencedobject.h"

#include "../thirdParty/MBES-lib/src/math/Distance.hpp"
#include <algorithm>

GeoreferencedObject::GeoreferencedObject(SidescanFile & file,SidescanImage & image,int x,int y,int pixelWidth,int pixelHeight) :
    file(file) ,
    image(image),
    startPing(*image.getPings().at( y)       ),
    endPing(*image.getPings().at( std::min((int) y+pixelHeight,(int) image.getPings().size())) ),
    x(x),
    y(y),
    pixelWidth(pixelWidth),
    pixelHeight(std::min((int)pixelHeight,(int)image.getPings().size()))
{
    yCenter = y + (pixelHeight/2);
    xCenter = x + (pixelWidth/2);

    computeDimensions();
    computePosition();
}

GeoreferencedObject::~GeoreferencedObject(){

}

void GeoreferencedObject::computeDimensions(){
    //FIXME: not as accurate as we could go, ok for ballpark

    if(yCenter < image.getPings().size()){
        width = pixelWidth * image.getPings().at(yCenter)->getDistancePerSample();
    }
    else{
        width = 0;
    }

    if( y < image.getPings().size() && y+pixelHeight < image.getPings().size() ){
        Position * pos1 = image.getPings()[y]->getPosition();
        Position * pos2  = image.getPings()[y+pixelHeight]->getPosition();

        height = Distance::haversine(pos1->getLongitude(),pos1->getLatitude(),pos2->getLongitude(),pos2->getLatitude());
    }
    else{
        height = 0;
    }

}

void GeoreferencedObject::computePosition(){
    //FIXME: this is not accurate, we need to take the across distance into account, this is just the tow fish's position along track

    if(yCenter < image.getPings().size()){
        SidescanPing * ping = image.getPings()[yCenter];
        position = ping->getPosition();
    }
    else{
        position = NULL;
    }
}
