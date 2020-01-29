#include "georeferencedobject.h"

#include "../thirdParty/MBES-lib/src/math/Distance.hpp"

#include "../thirdParty/MBES-lib/src/utils/Exception.hpp"

#include <algorithm>

GeoreferencedObject::GeoreferencedObject(SidescanFile & file,SidescanImage & image,int x,int y,int pixelWidth,int pixelHeight,std::string name, std::string description) :
    file(file) ,
    image(image),
    startPing(*image.getPings().at( y)       ),
    endPing(*image.getPings().at( std::min((int) y+pixelHeight,(int) image.getPings().size())) ),
    x(x),
    y(y),
    pixelWidth(pixelWidth),
    pixelHeight(std::min((int)pixelHeight,(int)image.getPings().size())),
    name(name),
    description(description)
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

        if ( pos1 == nullptr )
            throw new Exception( "GeoreferencedObject::computeDimensions(): pos1 is nullptr (y: " + std::to_string( y ) + ")" );

        Position * pos2  = image.getPings()[y+pixelHeight]->getPosition();

        if ( pos2 == nullptr )
            throw new Exception( "GeoreferencedObject::computeDimensions(): pos2 is nullptr (y: " + std::to_string( y )
                                    + ", pixelHeight: " + std::to_string( pixelHeight) + ")" );

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

    if ( position == nullptr )
        throw new Exception( "GeoreferencedObject::computePosition(): position is nullptr" );
}
