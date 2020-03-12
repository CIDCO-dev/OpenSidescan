#include "georeferencedobject.h"

#include "../thirdParty/MBES-lib/src/math/Distance.hpp"

#include "../thirdParty/MBES-lib/src/utils/Exception.hpp"
#include "../thirdParty/MBES-lib/src/math/CoordinateTransform.hpp"
#include "../thirdParty/MBES-lib/src/sidescan/SideScanGeoreferencing.hpp"

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
    if ( position != nullptr )
        delete position;

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
    if(yCenter < image.getPings().size()){
        SidescanPing * pingCenter = image.getPings()[yCenter];



        Eigen::Vector3d shipPositionEcef;
        CoordinateTransform::getPositionECEF(shipPositionEcef, *pingCenter->getPosition());

        Eigen::Vector3d tangentUnitVector;
        if(yCenter == image.getPings().size()-1) {

            //this is the last ping in the list compute tangent vector using previous ping
            SidescanPing * pingBefore = image.getPings()[yCenter-1];

            if(pingBefore->getTimestamp() > pingCenter->getTimestamp()) {
                throw new Exception("GeoreferencedObject::computePosition(): pingBefore [yCenter-1] has greater Timestamp than pingCenter");
            }

            Eigen::Vector3d positionBeforeECEF;
            CoordinateTransform::getPositionECEF(positionBeforeECEF, *pingBefore->getPosition());
            tangentUnitVector = (shipPositionEcef-positionBeforeECEF).normalized();
        } else {
            //compute tangent vector using next ping
            SidescanPing * pingAfter = image.getPings()[yCenter+1];

            if(pingAfter->getTimestamp() < pingCenter->getTimestamp()) {
                throw new Exception("GeoreferencedObject::computePosition(): pingAfter [yCenter+1] has lower Timestamp than pingCenter");
            }

            Eigen::Vector3d positionAfterECEF;
            CoordinateTransform::getPositionECEF(positionAfterECEF, *pingAfter->getPosition());
            tangentUnitVector = (positionAfterECEF-shipPositionEcef).normalized();
        }

        Eigen::Vector3d normalUnitVector = shipPositionEcef.normalized();

        Eigen::Vector3d starboardUnitVector = tangentUnitVector.cross(normalUnitVector);
        Eigen::Vector3d portUnitVector = -starboardUnitVector;

        double delta = pingCenter->getDistancePerSample();
        double distanceToObject = delta*yCenter;

        Eigen::Vector3d sideScanDistanceECEF;

        if(image.isStarboard()) {
            sideScanDistanceECEF = starboardUnitVector*distanceToObject;
        } else if(image.isPort()) {
            sideScanDistanceECEF = portUnitVector*distanceToObject;
        } else {
            position = new Position(
                        pingCenter->getPosition()->getTimestamp(),
                        pingCenter->getPosition()->getLatitude(),
                        pingCenter->getPosition()->getLongitude(),
                        pingCenter->getPosition()->getEllipsoidalHeight());
            return; // this image is neither port nor starboard. For now, use ship position
        }

        // TODO: get this lever arm from platform metadata
        Eigen::Vector3d antenna2TowPointEcef(0,0,0); // Zero vector for now

        // TODO: get this layback from xtf file
        Eigen::Vector3d laybackEcef(0,0,0); // Zero vector for now

        position = new Position(pingCenter->getTimestamp(), 0.0, 0.0, 0.0);

        //set position of this georeferenced object
        SideScanGeoreferencing::georeferenceSideScanEcef(shipPositionEcef, antenna2TowPointEcef, laybackEcef, sideScanDistanceECEF, *position);
    }
    else{
        position = NULL;
    }

    if ( position == nullptr )
        throw new Exception( "GeoreferencedObject::computePosition(): position is nullptr" );
}
