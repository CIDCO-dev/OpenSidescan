#include "inventoryobject.h"

#include "../../thirdParty/MBES-lib/src/math/Distance.hpp"

#include "../../thirdParty/MBES-lib/src/utils/Exception.hpp"
#include "../../thirdParty/MBES-lib/src/math/CoordinateTransform.hpp"
#include "../../thirdParty/MBES-lib/src/sidescan/SideScanGeoreferencing.hpp"
#include "../../thirdParty/MBES-lib/src/math/Distance.hpp"

#include <algorithm>

InventoryObject::InventoryObject(SidescanImage & image,int x,int y,int pixelWidth,int pixelHeight,std::string name, std::string description) :
    image(image),
    startPing(*image.getPings().at(y)       ),
    endPing(*image.getPings().at( std::min((int) y+pixelHeight,((int) image.getPings().size()) - 1 )) ),
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

InventoryObject::~InventoryObject(){
    if ( position != nullptr )
        delete position;

}

void InventoryObject::computeDimensions(){
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

        if ( pos1 && pos2  ){
            height = Distance::haversine(pos1->getLongitude(),pos1->getLatitude(),pos2->getLongitude(),pos2->getLatitude());
        }
        else{
            height = std::nan("");
        }
    }
    else{
        height = 0;
    }
}

void InventoryObject::computePosition(){

    int indexPingCenter = image.getPings().size() - yCenter;

    if(indexPingCenter > image.getPings().size()){
        position = nullptr;
        throw new Exception("GeoreferencedObject::computePosition(): position is nullptr" );
    }

    std::cout.precision(20);

    // Getting the ping of the middle of the image
    SidescanPing *pingCenter = image.getPings().at(indexPingCenter);

    // Getting attitude datas
    Attitude *attitude = pingCenter->getAttitude();

    // Getting positions datas
    Position *shipPosition = pingCenter->getPosition();
    Eigen::Vector3d shipPositionECEF;
    CoordinateTransform::getPositionECEF(shipPositionECEF, *shipPosition);

    // Creating the rotation matrix of transformation between NED and ECEF
    Eigen::Matrix3d ned2ecef;
    CoordinateTransform::ned2ecef(ned2ecef, *shipPosition);

    // Creating the rotation matrix of transformation between IMU and NED
    Eigen::Matrix3d imu2ned;
    CoordinateTransform::getDCM(imu2ned, *attitude);

    // Creating the rotation matrix of transformation between IMU and ECEF
    Eigen::Matrix3d imu2ecef = ned2ecef*imu2ned;

    // Getting antenna2TowPointEcef
    // TODO : change if no USBL to take in account this distance
    Eigen::Vector3d antenna2TowPointECEF;
    antenna2TowPointECEF[0] = 0;
    antenna2TowPointECEF[1] = 0;
    antenna2TowPointECEF[2] = 0;

    // Getting IMU base vector in the ECEF frame
    Eigen::Vector3d starboardUnitVectorIMU;
    starboardUnitVectorIMU[0] = 0;
    starboardUnitVectorIMU[1] = 1;
    starboardUnitVectorIMU[2] = 0;
    Eigen::Vector3d starboardUnitVectorECEF = imu2ecef*starboardUnitVectorIMU;

    Eigen::Vector3d portUnitVectorECEF = -starboardUnitVectorECEF;

    Eigen::Vector3d tangentUnitVectorIMU;
    tangentUnitVectorIMU[0] = 1;
    tangentUnitVectorIMU[1] = 0;
    tangentUnitVectorIMU[2] = 0;
    Eigen::Vector3d tangentUnitVectorECEF = imu2ecef*tangentUnitVectorIMU;

    Eigen::Vector3d downUnitVectorIMU;
    downUnitVectorIMU[0] = 0;
    downUnitVectorIMU[1] = 0;
    downUnitVectorIMU[2] = 1;
    Eigen::Vector3d downUnitVectorECEF = imu2ecef*downUnitVectorIMU;

    // Getting layback vector
    double layback = pingCenter->getLayback();
    Eigen::Vector3d laybackECEF = -layback*tangentUnitVectorECEF;

    // Getting distance to object
    //double slantRange = pingCenter->getSlantRange();
    //double distanceToObject = indexPingCenter*slantRange/image.getPings().size(); // px * m / px
    double distancePerSample = pingCenter->getDistancePerSample();
    double distanceToObject = distancePerSample*indexPingCenter/2;

    // Getting angle between nadir and object
    //double sensorPrimaryAltitude = pingCenter->getSensorPrimaryAltitude();
    //double thetaRadians = acos(sensorPrimaryAltitude/distanceToObject);

    // Getting time corresponding to 1 pixel
    // double timeDuration = pingCenter->getTimeDuration();

    // Getting soundVelocity
    // double soundVelocity = pingCenter->getSoundVelocity();

    /*
    std::cout<<std::endl<<"sensorPrimaryAltitude "<<sensorPrimaryAltitude;
    std::cout<<std::endl<<"distanceToObject "<<distanceToObject;
    std::cout<<std::endl<<"thetaRadians "<<thetaRadians;

    Eigen::Vector3d ObjectDirectionUnitVector;
    if(image.isStarboard()) {
        ObjectDirectionUnitVector = cos(thetaRadians)*downUnitVector + sin(thetaRadians)*starboardUnitVector;
    } else if(image.isPort()) {
        ObjectDirectionUnitVector = cos(thetaRadians)*downUnitVector + sin(thetaRadians)*portUnitVector;
    } else {
        position = new Position(pingCenter->getTimestamp(), 0.0, 0.0, 0.0);
        CoordinateTransform::convertECEFToLongitudeLatitudeElevation(shipPositionECEF, *position);
        return; // This image is neither port nor starboard. We use ship position, it is the most accurate you can have.
    }

    Eigen::Vector3d sideScanDistanceECEF = distanceToObject*ObjectDirectionUnitVector;
    */

    double sensorPrimaryAltitude = pingCenter->getSensorPrimaryAltitude();
    double groundDistance2 = pow(distanceToObject, 2) - pow(sensorPrimaryAltitude, 2);
    if (groundDistance2 < 0) {
        // TODO : take in account beamAngle to locate object inside water column.
        position = new Position(pingCenter->getTimestamp(), 0.0, 0.0, 0.0);
        CoordinateTransform::convertECEFToLongitudeLatitudeElevation(shipPositionECEF, *position);
        std::cerr<<"Object inside water column"<<std::endl;
        return;
    }
    Eigen::Vector3d sideScanDistanceECEF;
    double groundDistance = sqrt(groundDistance2);
    if(image.isStarboard()) {
        sideScanDistanceECEF = groundDistance*starboardUnitVectorECEF + sensorPrimaryAltitude*downUnitVectorECEF;
    } else if(image.isPort()) {
        sideScanDistanceECEF = groundDistance*portUnitVectorECEF + sensorPrimaryAltitude*downUnitVectorECEF;
    } else {
        position = new Position(pingCenter->getTimestamp(), 0.0, 0.0, 0.0);
        CoordinateTransform::convertECEFToLongitudeLatitudeElevation(shipPositionECEF, *position);
        return; // This image is neither port nor starboard. We use ship position, it is the most accurate you can have.
    }

    /*
    Eigen::Vector3d sideScanDistanceECEF;
    if(image.isStarboard()) {
        sideScanDistanceECEF = distanceToObject*starboardUnitVector;
    } else if(image.isPort()) {
        sideScanDistanceECEF = distanceToObject*portUnitVector;
    } else {
        position = new Position(pingCenter->getTimestamp(), 0.0, 0.0, 0.0);
        CoordinateTransform::convertECEFToLongitudeLatitudeElevation(shipPositionECEF, *position);
        return; // This image is neither port nor starboard. We use ship position, it is the most accurate you can have.
    }
    */

    Eigen::Vector3d objectPositionEcef = shipPositionECEF + antenna2TowPointECEF + laybackECEF + sideScanDistanceECEF;

    position = new Position(pingCenter->getTimestamp(), 0.0, 0.0, 0.0);
    CoordinateTransform::convertECEFToLongitudeLatitudeElevation(objectPositionEcef, *position);

    // TODO : remove what comes next

    /*
    if(image.isStarboard()) {
        std::cout<<"starboard side"<<std::endl;
    } else if(image.isPort()) {
        std::cout<<"port side"<<std::endl;
    } else {
        std::cout<<"This image is neither port nor starboard."<<std::endl;
    }
    */

    shipPosition = new Position(pingCenter->getTimestamp(), 0.0, 0.0, 0.0);
    CoordinateTransform::convertECEFToLongitudeLatitudeElevation(shipPositionECEF, *shipPosition);

    /*
    std::cout<<"x"<<std::endl<<x<<std::endl;
    std::cout<<"y"<<std::endl<<y<<std::endl;
    std::cout<<"pixelWidth"<<std::endl<<pixelWidth<<std::endl;
    std::cout<<"pixelHeight"<<std::endl<<pixelHeight<<std::endl;
    std::cout<<"xCenter"<<std::endl<<xCenter<<std::endl;
    std::cout<<"yCenter"<<std::endl<<yCenter<<std::endl;
    std::cout<<"indexPingCenter"<<std::endl<<indexPingCenter<<std::endl;
    std::cout<<*attitude;
    std::cout<<"ned2ecef"<<std::endl<<ned2ecef<<std::endl;
    std::cout<<"imu2ned"<<std::endl<<imu2ned<<std::endl;
    std::cout<<"tangentUnitVector"<<std::endl<<tangentUnitVector<<std::endl;
    std::cout<<"portUnitVector"<<std::endl<<portUnitVector<<std::endl;
    std::cout<<"starboardUnitVector"<<std::endl<<starboardUnitVector<<std::endl;
    //std::cout<<"Distancepersample "<<distancePerSample<<std::endl;
    std::cout<<"distanceToObject"<<std::endl<<distanceToObject<<std::endl;
    std::cout<<"layback "<<layback<<std::endl;
    std::cout<<"starboardUnitVector"<<std::endl<<starboardUnitVector<<std::endl;
    std::cout<<"portUnitVector"<<std::endl<<portUnitVector<<std::endl;
    std::cout<<"shipPositionEcef"<<std::endl<<shipPositionEcef<<std::endl;
    std::cout<<"sideScanDistanceECEF"<<std::endl<<sideScanDistanceECEF<<std::endl;
    std::cout<<"objectPositionEcef"<<std::endl<<objectPositionEcef<<std::endl;
    std::cout<<"ShipPosition"<<std::endl<<shipPosition->getLatitude()<<" "<<shipPosition->getLongitude()<<" "<<shipPosition->getEllipsoidalHeight()<<std::endl;
    std::cout<<std::endl<<shipPosition->getLatitude()<<" "<<shipPosition->getLongitude()<<" "<<shipPosition->getEllipsoidalHeight()<<" 0";
    */

    std::cout<<std::endl<<"objectPosition "<<position->getLatitude()<<" "<<position->getLongitude()<<" "<<position->getEllipsoidalHeight();
    std::cout<<std::endl<<"shipPosition "<<shipPosition->getLatitude()<<" "<<shipPosition->getLongitude()<<" "<<shipPosition->getEllipsoidalHeight();
    //std::cout<<std::endl<<"sensorPrimaryAltitude"<<pingCenter->getSensorPrimaryAltitude()<<std::endl;
    /*
    for (int i=0 ; i < image.getPings().size() ; i=i+20 ) {
        Position *p = image.getPings()[i]->getPosition();
        std::cout<<std::endl<<i<<" "<<p->getLatitude()<<" "<<p->getLongitude()<<" "<<p->getEllipsoidalHeight();
    }
    */

}

bool InventoryObject::is_inside(struct region & area){
    /*
    std::cout<<"area:"<<"\n";
    std::cout<<area.x<<" "<<area.y<<" "<<area.width<<" "<<area.height<<"\n";
    std::cout<<"invetory object:"<<"\n";
    std::cout<<this->getX()<<" "<<this->getY()<<" "<<this->getX() + this->getPixelWidth()<<" "<<this->getY() + this->getPixelHeight()<<"\n";
    */
    if(
            (area.x <= this->getX()) && (area.width >= (this->getX() + this->getPixelWidth())) &&
            (area.y <= this->getY()) && (area.height >= (this->getY() + this->getPixelHeight()))
    ){
        return true;
    }
    else{
        return false;
    }
}
