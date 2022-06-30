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

    int indexPingCenter = image.getPings().size() - yCenter;
    if(indexPingCenter < image.getPings().size()){
        width = pixelWidth * image.getPings().at(indexPingCenter)->getDistancePerSample();
    }
    else{
        width = 0;
    }

    int indexPingBeginning = image.getPings().size() - y;
    int indexPingEnd = image.getPings().size() - (y + pixelHeight);

    if( indexPingBeginning < image.getPings().size() && indexPingEnd < image.getPings().size() ){

        // Getting the pings
        SidescanPing *pingBeginning = image.getPings().at(indexPingBeginning);
        SidescanPing *pingEnd = image.getPings().at(indexPingEnd);

        // Getting attitude datas
        Attitude *attitudeBeginning = pingBeginning->getAttitude();
        Attitude *attitudeEnd = pingEnd->getAttitude();

        // Getting positions datas
        Position *shipPositionBeginning = pingBeginning->getPosition();
        Position *shipPositionEnd = pingEnd->getPosition();
        Eigen::Vector3d shipPositionEcefBeginning;
        CoordinateTransform::getPositionECEF(shipPositionEcefBeginning, *shipPositionBeginning);
        Eigen::Vector3d shipPositionEcefEnd;
        CoordinateTransform::getPositionECEF(shipPositionEcefEnd, *shipPositionEnd);

        // Creating the rotation matrix of transformation between NED and Ecef
        // We use the same matrix for all the pings because its variations are small
        Eigen::Matrix3d ned2Ecef;
        CoordinateTransform::ned2ecef(ned2Ecef, *shipPositionBeginning);

        // Creating the rotation matrix of transformation between IMU and NED
        Eigen::Matrix3d imu2nedBeginning;
        CoordinateTransform::getDCM(imu2nedBeginning, *attitudeBeginning);
        Eigen::Matrix3d imu2nedEnd;
        CoordinateTransform::getDCM(imu2nedEnd, *attitudeEnd);

        // Creating the horizontal vector in the direction of the ping
        Eigen::Vector3d lateralUnitVectorIMU;
        lateralUnitVectorIMU[0] = 0;
        lateralUnitVectorIMU[2] = 0;
        if(image.isStarboard()) {
            lateralUnitVectorIMU[1] = 1;
        } else if(image.isPort()) {
            lateralUnitVectorIMU[1] = -1;
        } else {
            lateralUnitVectorIMU[1] = 0; // We will just return the distance between the two ship positions
        }

        // Getting distances to object
        double distancePerSampleBeginning = pingBeginning->getDistancePerSample();
        double distanceToObjectBeginning = distancePerSampleBeginning*indexPingBeginning/2;
        double distancePerSampleEnd = pingEnd->getDistancePerSample();
        double distanceToObjectEnd = distancePerSampleEnd*indexPingEnd/2;

        // Creating the vector between the beginning and the end of the image
        Eigen::Vector3d posBeginningEcef = shipPositionEcefBeginning + distanceToObjectBeginning*ned2Ecef*imu2nedBeginning*lateralUnitVectorIMU;
        Eigen::Vector3d posEndEcef = shipPositionEcefEnd + distanceToObjectEnd*ned2Ecef*imu2nedEnd*lateralUnitVectorIMU;
        Eigen::Vector3d heightVector = posBeginningEcef-posEndEcef;
        height = heightVector.norm();
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
    Eigen::Vector3d shipPositionEcef;
    CoordinateTransform::getPositionECEF(shipPositionEcef, *shipPosition);

    // Creating the rotation matrix of transformation between NED and Ecef
    Eigen::Matrix3d ned2Ecef;
    CoordinateTransform::ned2ecef(ned2Ecef, *shipPosition);

    // Creating the rotation matrix of transformation between IMU and NED
    Eigen::Matrix3d imu2ned;
    CoordinateTransform::getDCM(imu2ned, *attitude);

    // Creating the rotation matrix of transformation between IMU and ECEF
    Eigen::Matrix3d imu2Ecef = ned2Ecef*imu2ned;

    // Getting IMU base vector in the Ecef frame
    Eigen::Vector3d starboardUnitVectorIMU;
    starboardUnitVectorIMU[0] = 0;
    starboardUnitVectorIMU[1] = 1;
    starboardUnitVectorIMU[2] = 0;
    Eigen::Vector3d starboardUnitVectorEcef = imu2Ecef*starboardUnitVectorIMU;

    Eigen::Vector3d portUnitVectorEcef = -starboardUnitVectorEcef;

    Eigen::Vector3d tangentUnitVectorIMU;
    tangentUnitVectorIMU[0] = 1;
    tangentUnitVectorIMU[1] = 0;
    tangentUnitVectorIMU[2] = 0;
    Eigen::Vector3d tangentUnitVectorEcef = imu2Ecef*tangentUnitVectorIMU;

    Eigen::Vector3d downUnitVectorIMU;
    downUnitVectorIMU[0] = 0;
    downUnitVectorIMU[1] = 0;
    downUnitVectorIMU[2] = 1;
    Eigen::Vector3d downUnitVectorEcef = imu2Ecef*downUnitVectorIMU;

    // Getting layback vector
    double layback = pingCenter->getLayback();
    Eigen::Vector3d laybackEcef = -layback*tangentUnitVectorEcef;

    // Getting distance to object
    //double slantRange = pingCenter->getSlantRange();
    //double distanceToObject = indexPingCenter*slantRange/image.getPings().size(); // px * m / px
    double distancePerSample = pingCenter->getDistancePerSample();
    double distanceToObject = distancePerSample*indexPingCenter/2;

    double sensorPrimaryAltitude = pingCenter->getSensorPrimaryAltitude();
    double groundDistance2 = pow(distanceToObject, 2) - pow(sensorPrimaryAltitude, 2);
    if (groundDistance2 < 0) {
        // TODO : take in account beamAngle to locate object inside water column.
        position = new Position(pingCenter->getTimestamp(), 0.0, 0.0, 0.0);
        CoordinateTransform::convertECEFToLongitudeLatitudeElevation(shipPositionEcef, *position);
        std::cerr<<"Object inside water column"<<std::endl;
        return;
    }
    Eigen::Vector3d sideScanDistanceEcef;
    double groundDistance = sqrt(groundDistance2);
    if(image.isStarboard()) {
        sideScanDistanceEcef = groundDistance*starboardUnitVectorEcef + sensorPrimaryAltitude*downUnitVectorEcef;
    } else if(image.isPort()) {
        sideScanDistanceEcef = groundDistance*portUnitVectorEcef + sensorPrimaryAltitude*downUnitVectorEcef;
    } else {
        position = new Position(pingCenter->getTimestamp(), 0.0, 0.0, 0.0);
        CoordinateTransform::convertECEFToLongitudeLatitudeElevation(shipPositionEcef, *position);
        return; // This image is neither port nor starboard. We use ship position, it is the most accurate you can have.
    }

    Eigen::Vector3d antenna2TowPointEcef;
    antenna2TowPointEcef[0] = 0;
    antenna2TowPointEcef[1] = 0;
    antenna2TowPointEcef[2] = 0;

    Eigen::Vector3d objectPositionEcef = shipPositionEcef + antenna2TowPointEcef + laybackEcef + sideScanDistanceEcef;

    position = new Position(pingCenter->getTimestamp(), 0.0, 0.0, 0.0);
    CoordinateTransform::convertECEFToLongitudeLatitudeElevation(objectPositionEcef, *position);

    shipPosition = new Position(pingCenter->getTimestamp(), 0.0, 0.0, 0.0);
    CoordinateTransform::convertECEFToLongitudeLatitudeElevation(shipPositionEcef, *shipPosition);

    //std::cout<<std::endl<<"objectPosition "<<position->getLatitude()<<" "<<position->getLongitude()<<" "<<position->getEllipsoidalHeight();
    //std::cout<<std::endl<<"shipPosition "<<shipPosition->getLatitude()<<" "<<shipPosition->getLongitude()<<" "<<shipPosition->getEllipsoidalHeight();
    //std::cout<<std::endl<<"sensorPrimaryAltitude"<<pingCenter->getSensorPrimaryAltitude()<<std::endl;

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
