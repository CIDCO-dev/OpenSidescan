#include "inventoryobject.h"

#include "../../thirdParty/MBES-lib/src/math/Distance.hpp"

#include "../../thirdParty/MBES-lib/src/utils/Exception.hpp"
#include "../../thirdParty/MBES-lib/src/math/CoordinateTransform.hpp"
#include "../../thirdParty/MBES-lib/src/sidescan/SideScanGeoreferencing.hpp"
#include "../../thirdParty/MBES-lib/src/math/Distance.hpp"
#include "../../thirdParty/WorldMagneticModel/WMM2020_Linux/src/wmm_calculations.c"
#include <algorithm>
#include <math.h>

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
    //FIXME: Apply same modifications than to computePosition

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
    //FIXME : Doesn't return the true position.
    //The computation of the distance between sidescan and object doesn't return the good result.

    //As the image is a waterfall, it is reversed
    int indexPingCenter = image.getPings().size() - yCenter;

    if(indexPingCenter >= image.getPings().size() || indexPingCenter < 0){
        position = nullptr;
        throw new Exception("GeoreferencedObject::computePosition(): position is nullptr" );
    }

    //TODO : remove when we don't need to make tests anymore
    std::cout.precision(20);
    std::cerr.precision(20);

    // Getting the ping of the middle of the image
    SidescanPing *pingCenter = image.getPings().at(indexPingCenter);

    // The x axis is reversed if the image is port
    int indexObjectDistance;
    int pingCenterSize = pingCenter->getSamples().size();
    if(image.isStarboard()) {
        indexObjectDistance = x;
    } else if(image.isPort()) {
        indexObjectDistance = pingCenterSize - x;
    } else {
        indexObjectDistance = 0;
    }

    // Getting attitude datas
    Attitude *attitude = pingCenter->getAttitude();

    // Getting positions datas
    Position *shipPosition = pingCenter->getPosition();
    Eigen::Vector3d shipPositionEcef;
    CoordinateTransform::getPositionECEF(shipPositionEcef, *shipPosition);

    // Creating the rotation matrix of transformation between NED and Ecef
    Eigen::Matrix3d ned2Ecef;
    CoordinateTransform::ned2ecef(ned2Ecef, *shipPosition);

    double ShipLongitude = shipPosition->getLongitude();
    double ShipLatitude = shipPosition->getLatitude();
    double shipEllipsoidalHeight = shipPosition->getEllipsoidalHeight();
    double tYear = 1970 + shipPosition->getTimestamp()/pow(10, 6)/60/60/24/365.2425;
    char filename[] = "/home/blanc-sablon/Documents/GitHub/OpenSidescan/src/thirdParty/WorldMagneticModel/WMM2020_Linux/src/WMM.COF";
    MAGtype_GeoMagneticElements magneticModel = getMagneticModel(ShipLongitude, ShipLatitude, shipEllipsoidalHeight, tYear, filename);

    double declinationDegree = magneticModel.Decl; /* 1. Angle between the magnetic field vector and true north, positive east*/
    double inclinationDegree = magneticModel.Incl; /*2. Angle between the magnetic field vector and the horizontal plane, positive down*/

    Eigen::Matrix3d mag2geoNorth;
    CoordinateTransform::magneticNorth2geographicNorth(mag2geoNorth, declinationDegree, inclinationDegree);

    // Creating the rotation matrix of transformation between IMU and NED
    Eigen::Matrix3d imu2ned;
    CoordinateTransform::getDCM(imu2ned, *attitude);

    // Creating the rotation matrix of transformation between IMU and ECEF
    Eigen::Matrix3d imu2Ecef = ned2Ecef*mag2geoNorth*imu2ned;
    //Eigen::Matrix3d imu2Ecef = ned2Ecef*imu2ned;

    //int test = getMagneticAngles(30, 45, 0, 2021);

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
    double distanceToObject = 2*distancePerSample*indexObjectDistance;

    // Initialising the position of the sigmoid step
    // We take the index of the first pixel that is superior to k

    std::vector<double> rawSamples = pingCenter->getRawSamples();
    int nSamples = rawSamples.size();

    std::vector<double> sortedRawSamples = rawSamples;
    std::nth_element(sortedRawSamples.begin(),
                     sortedRawSamples.begin() + nSamples / 2,
                     sortedRawSamples.end());
    int middleIndex = nSamples/2;
    double k = sortedRawSamples[middleIndex];

    //double samplesMean = samplesSum/nSamples;
    double sensorPrimaryAltitudePixels;
    if(image.isStarboard()) {
        for (int i=100; i<rawSamples.size(); i++) {
            if (rawSamples[i] > k) {
                sensorPrimaryAltitudePixels = i;
                break;
            }
        }
    } else if(image.isPort()) {
        for (int i=100; i<rawSamples.size(); i++) {
            if (rawSamples[nSamples-i] > k) {
                sensorPrimaryAltitudePixels = i;
                break;
            }
        }
    }

    // Least squares calculation of the size of the water column
    double nSigmoid = 10; // Scale factor on the x axis
    // Non linear least squares loop
    double correction = std::numeric_limits<double>::infinity();
    while (correction<1/10) { // the unit of correction is the pixel
        // Initialising least squares matrixes
        int nx = 1;
        Eigen::MatrixXf A(nSamples, nx);
        Eigen::VectorXf b(nSamples);
        Eigen::VectorXf x(nx);
        for (int j=0; j<nSamples; j++) {
            double delta;
            if(image.isStarboard()) {
                delta = j-sensorPrimaryAltitudePixels;
            } else if(image.isPort()) {
                delta = -j+nSamples-sensorPrimaryAltitudePixels;
            };
            double e = std::exp( -nSigmoid * delta );
            double f = k / ( 1 + e );
            double dfdd = - k * nSigmoid * e / pow( 1 + e , 2);
            b(j, 0) = rawSamples[j] - f;
            A(j, 0) = dfdd;
        };
        // Calculating least squares solution
        x = (A.transpose() * A).ldlt().solve(A.transpose() * b);
        correction = x(0, 0);
        sensorPrimaryAltitudePixels = sensorPrimaryAltitudePixels + correction;
    }
    double sensorPrimaryAltitude = distancePerSample*sensorPrimaryAltitudePixels;

    // loop only used to write datas in text file
    for (int j=0; j<nSamples; j++) {
        double delta;
        if(image.isStarboard()) {
            delta = j-sensorPrimaryAltitudePixels;
        } else if(image.isPort()) {
            delta = -j+nSamples-sensorPrimaryAltitudePixels;
        };
        double e = std::exp( -nSigmoid * delta );
        double f = k / ( 1 + e );
        double dfdd = - k * nSigmoid * e / pow( 1 + e , 2);
    };

    double groundDistance2 = pow(distanceToObject, 2) - pow(sensorPrimaryAltitude, 2);
    if (groundDistance2 < 0) {
        // TODO : take in account beamAngle to locate object inside water column.
        position = new Position(pingCenter->getTimestamp(), 0.0, 0.0, 0.0);
        CoordinateTransform::convertECEFToLongitudeLatitudeElevation(shipPositionEcef, *position);
        std::cerr<<std::endl<<"Object inside water column"<<std::endl;
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

    // TODO : change the value of this vector if no usbl
    Eigen::Vector3d antenna2TowPointEcef;
    antenna2TowPointEcef[0] = 0;
    antenna2TowPointEcef[1] = 0;
    antenna2TowPointEcef[2] = 0;

    Eigen::Vector3d objectPositionEcef = shipPositionEcef + antenna2TowPointEcef + laybackEcef + sideScanDistanceEcef;

    position = new Position(pingCenter->getTimestamp(), 0.0, 0.0, 0.0);
    CoordinateTransform::convertECEFToLongitudeLatitudeElevation(objectPositionEcef, *position);

    shipPosition = new Position(pingCenter->getTimestamp(), 0.0, 0.0, 0.0);
    CoordinateTransform::convertECEFToLongitudeLatitudeElevation(shipPositionEcef, *shipPosition);
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
