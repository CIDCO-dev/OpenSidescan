#ifndef GEOREFERENCEDOBJECT_H
#define GEOREFERENCEDOBJECT_H

#include "sidescan/sidescanfile.h"
#include "sidescan/sidescanimage.h"

#include "../thirdParty/MBES-lib/src/sidescan/SidescanPing.hpp"

class SidescanFile;
class SidescanImage;


class GeoreferencedObject
{
public:
    GeoreferencedObject(SidescanFile & file,SidescanImage & image,int x,int y,int pixelWidth,int pixelHeight,std::string name="Unknown",std::string description = "");

    ~GeoreferencedObject();

    SidescanFile  & getFile()       { return file       ;};
    SidescanImage & getImage()      { return image      ;};
    SidescanPing  & getStartPing()  { return startPing  ;};
    SidescanPing  & getEndPing()    { return endPing    ;};

    Position * getPosition(){ return position; }

    double getWidth() { return width;}
    double getHeight(){ return height;}

    int getX(){ return x;}
    int getY(){ return y;}

    int getXCenter(){ return xCenter;}
    int getYCenter(){ return yCenter;}

    int getPixelWidth(){ return pixelWidth;}
    int getPixelHeight(){ return pixelHeight;}

    std::string & getName(){ return name;}
    void setName(std::string & newName){ name = newName;}

    std::string & getDescription() { return description;}
    void setDescription(std::string & newDescription) { description = newDescription;}


private:
    int x; //top left corner
    int y;

    int xCenter; //center
    int yCenter;

    int pixelWidth;   //in pixels
    int pixelHeight;

    double width; //in meters
    double height;

    Position * position;
    void computePosition();
    void computeDimensions();

    std::string   name;
    std::string   description;


    SidescanFile  & file;
    SidescanImage & image;
    SidescanPing  & startPing;
    SidescanPing  & endPing;
};

#endif // GEOREFERENCEDOBJECT_H
