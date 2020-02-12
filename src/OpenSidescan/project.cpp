#include "project.h"

#include <QFile>
#include <QtXml>
#include <QPixmap>

#include <cstring>


#include <set>
#include <utility>      // std::pair, std::make_pair

#include <vector>

#include "opencv2/core.hpp"
#include "../thirdParty/opencv/apps/createsamples/utility.hpp"



#include "sidescanimager.h"
#include "qthelper.h"

// For ordering the set
struct compare
{
    bool operator()( const std::pair<int,int> & lhs, const std::pair<int,int> & rhs ) const
    {

        if ( lhs.first > rhs.first )
            return false;

        if ( lhs.first < rhs.first )
            return true;

        // First is equal, look at second

        if ( lhs.second <= rhs.second )
            return true;
        else
            return false;
    }
};



Project::Project()
{

}

Project::~Project(){
    for(auto i=this->getFiles().begin();i!=this->getFiles().end();i++){
        delete (*i);
    }
}

void Project::read(std::string & filename){
    QFile file(QString::fromStdString(filename));
    file.open(QIODevice::ReadOnly);

    QXmlStreamReader xml(&file);

    std::string currentImage;
    SidescanFile * currentFile=NULL;

    while(!xml.atEnd()){

        //Read through file
        switch(xml.readNext()){
            case QXmlStreamReader::StartElement:
                std::string name = xml.name().toString().toStdString();

                //Handle different element types
                if(strncmp(name.c_str(),"File",4)==0){
                    //Sidescan file
                    std::string filename = xml.attributes().value(QString::fromStdString("filename")).toString().toStdString();

                    SidescanImager imager;
                    DatagramParser * parser = DatagramParserFactory::build(filename,imager);
                    parser->parse(filename);
                    currentFile = imager.generate(filename);

                    this->getFiles().push_back(currentFile);

                    currentImage = "";

                    delete parser;
                }
                else if(strncmp(name.c_str(),"Image",5)==0){
                    //Sidescan image
                    currentImage=xml.attributes().value(QString::fromStdString("channelName")).toString().toStdString();
                }
                else if(strncmp(name.c_str(),"Object",5)==0){
                    //Inventory Objects
                    if(currentFile){
                        for(auto i = currentFile->getImages().begin();i!=currentFile->getImages().end();i++){
                            if(strncmp((*i)->getChannelName().c_str(),currentImage.c_str(),currentImage.size())==0){
                                //instanciate object
                                int x                   = std::stoi(xml.attributes().value(QString::fromStdString("x")).toString().toStdString());
                                int y                   = std::stoi(xml.attributes().value(QString::fromStdString("y")).toString().toStdString());
                                int pixelWidth          = std::stoi(xml.attributes().value(QString::fromStdString("pixelWidth")).toString().toStdString());
                                int pixelHeight         = std::stoi(xml.attributes().value(QString::fromStdString("pixelHeight")).toString().toStdString());
                                std::string name        = xml.attributes().value(QString::fromStdString("name")).toString().toStdString();
                                std::string description = xml.attributes().value(QString::fromStdString("description")).toString().toStdString();

                                GeoreferencedObject * object = new GeoreferencedObject(*currentFile,*(*i),x,y,pixelWidth,pixelHeight,name,description);
                                (*i)->getObjects().push_back(object);
                            }
                        }

                    }
                    else{
                        //No file...wtf
                        std::cerr << "Malformed Project File: No file associated with object" << std::endl;
                    }
                }
            break;
        }
    }

}

void Project::write(std::string & filename){
    QFile file(QString::fromStdString(filename));
    file.open(QIODevice::WriteOnly);

    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();

    xmlWriter.writeStartElement("Project");

    for(auto i=files.begin();i!=files.end();i++){
        xmlWriter.writeStartElement("File");

        //TODO: use relative file paths
        xmlWriter.writeAttribute(QString::fromStdString("filename"),QString::fromStdString((*i)->getFilename()));

        for(auto j=(*i)->getImages().begin();j!=(*i)->getImages().end();j++){
            //TODO: write objects
            xmlWriter.writeStartElement("Image");

            xmlWriter.writeAttribute(QString::fromStdString("channelName"), QString::fromStdString((*j)->getChannelName()) );

            for(auto k = (*j)->getObjects().begin(); k != (*j)->getObjects().end(); k++){
                xmlWriter.writeStartElement("Object");

                xmlWriter.writeAttribute(QString::fromStdString("x"),           QString::fromStdString( std::to_string((*k)->getX())            ) );
                xmlWriter.writeAttribute(QString::fromStdString("y"),           QString::fromStdString( std::to_string((*k)->getY())            ) );
                xmlWriter.writeAttribute(QString::fromStdString("pixelWidth"),  QString::fromStdString( std::to_string((*k)->getPixelWidth())   ) );
                xmlWriter.writeAttribute(QString::fromStdString("pixelHeight"), QString::fromStdString( std::to_string((*k)->getPixelHeight())  ) );
                xmlWriter.writeAttribute(QString::fromStdString("name"),        QString::fromStdString( (*k)->getName()                         ) );
                xmlWriter.writeAttribute(QString::fromStdString("description"), QString::fromStdString( (*k)->getDescription()                  ) );

                xmlWriter.writeEndElement();
            }

            xmlWriter.writeEndElement();
        }

        xmlWriter.writeEndElement();
    }

    xmlWriter.writeEndElement();

    file.close();
}


void Project::exportInventoryAsKml(std::string & filename){
    QFile file(QString::fromStdString(filename));
    file.open(QIODevice::WriteOnly);

    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();

    xmlWriter.writeStartElement("kml");
    xmlWriter.writeNamespace(QString::fromStdString("http://www.opengis.net/kml/2.2"));
    xmlWriter.writeStartElement("Document");

    for(auto i=files.begin();i!=files.end();i++){
        for(auto j=(*i)->getImages().begin();j!=(*i)->getImages().end();j++){
            for(auto k=(*j)->getObjects().begin();k!=(*j)->getObjects().end();k++){

                if((*k)->getPosition()){
                    xmlWriter.writeStartElement("Placemark");

                    //name
                    xmlWriter.writeStartElement("name");
                    xmlWriter.writeCharacters(QString::fromStdString((*k)->getName()));
                    xmlWriter.writeEndElement();

                    //description
                    xmlWriter.writeStartElement("description");
                    xmlWriter.writeCDATA(QString::fromStdString((*k)->getDescription()));
                    xmlWriter.writeEndElement();

                    //Point coordinates
                    std::stringstream ss;
                    ss << std::setprecision(15);
                    ss << (*k)->getPosition()->getLongitude() << "," << (*k)->getPosition()->getLatitude() ;

                    xmlWriter.writeStartElement("Point");

                    xmlWriter.writeStartElement("coordinates");
                    xmlWriter.writeCharacters(QString::fromStdString(ss.str()));
                    xmlWriter.writeEndElement();

                    xmlWriter.writeEndElement();


                    xmlWriter.writeEndElement();
                }
            }
        }
    }

    xmlWriter.writeEndElement();
    xmlWriter.writeEndElement();

    file.close();
}

//void Project::saveObjectImages( const QString & folder )
void Project::saveObjectImages( const QString & absolutePath,
                       const QString & fileNameWithoutExtension )
{
//    std::cout << "\nBeginning of Project::saveObjectImages()\n"
//        << "Folder: \"" << folder.toStdString() << "\"\n" << std::endl;


    // Open file, write beginning of the file

//    QString fileNameHTML = folder + "/" + "description.html";

    QString fileNameHTML = absolutePath + "/" + fileNameWithoutExtension + ".html";

    QFile file( fileNameHTML );
    bool isfileForHTMLopened = file.open(QIODevice::WriteOnly);

    QXmlStreamWriter xmlWriter(&file);

    if( isfileForHTMLopened )
    {
        xmlWriter.setAutoFormatting(true);
        xmlWriter.writeStartDocument();

        xmlWriter.writeDTD( "<!DOCTYPE html>" );

        xmlWriter.writeStartElement("html");

        // Style

        xmlWriter.writeStartElement("head");
        xmlWriter.writeStartElement("style");

        xmlWriter.writeCharacters( "table, th, td {\n" );
        xmlWriter.writeCharacters( "  border: 1px solid black;\n" );
        xmlWriter.writeCharacters( "  border-collapse: collapse;\n" );
        xmlWriter.writeCharacters( "}\n" );
        xmlWriter.writeCharacters( "th, td {\n" );
        xmlWriter.writeCharacters( "  padding: 5px;\n" );
        xmlWriter.writeCharacters( "}\n" );
        xmlWriter.writeCharacters( "th {\n" );
        xmlWriter.writeCharacters( "  text-align: left;\n" );
        xmlWriter.writeCharacters( "}\n" );

        xmlWriter.writeEndElement(); // style
        xmlWriter.writeEndElement(); // head

        // Body

        xmlWriter.writeStartElement("body");

        xmlWriter.writeStartElement("h2"); // Left-align Headings
        xmlWriter.writeCharacters( "Objects" );
        xmlWriter.writeEndElement();

        xmlWriter.writeStartElement("p");
        xmlWriter.writeCharacters( "List of objects" );
        xmlWriter.writeEndElement();

        xmlWriter.writeStartElement( "table style=\"width:100%\"" );

        // Table header
        xmlWriter.writeStartElement("tr");

        xmlWriter.writeStartElement("th");
        xmlWriter.writeCharacters( "Name" );
        xmlWriter.writeEndElement();

        xmlWriter.writeStartElement("th");
        xmlWriter.writeCharacters( "File" );
        xmlWriter.writeEndElement();

        xmlWriter.writeStartElement("th");
        xmlWriter.writeCharacters( "Channel" );
        xmlWriter.writeEndElement();

        xmlWriter.writeStartElement("th");
        xmlWriter.writeCharacters( "Longitude" );
        xmlWriter.writeEndElement();

        xmlWriter.writeStartElement("th");
        xmlWriter.writeCharacters( "Latitude" );
        xmlWriter.writeEndElement();

        xmlWriter.writeStartElement("th");
        xmlWriter.writeCharacters( "Width (m)" );
        xmlWriter.writeEndElement();

        xmlWriter.writeStartElement("th");
        xmlWriter.writeCharacters( "Height (m)" );
        xmlWriter.writeEndElement();

        xmlWriter.writeStartElement("th");
        xmlWriter.writeCharacters( "Image" );
        xmlWriter.writeEndElement();


        xmlWriter.writeEndElement(); // tr
    }


    // i is an iterator to a ( SidescanFile * )
    for(auto i = files.begin(); i != files.end(); ++i){

        // j is an iterator to a (SidescanImage* )
        for(auto j=(*i)->getImages().begin();j!=(*i)->getImages().end();j++){

            // k is an iterator to (GeoreferencedObject *)
            for(auto k=(*j)->getObjects().begin();k!=(*j)->getObjects().end();k++){

                // Copy the part of the cv::Mat with the object into a new cv::Mat
                cv::Mat objectMat;
                (*j)->getImage()( cv::Rect( (*k)->getX(), (*k)->getY(), (*k)->getPixelWidth(), (*k)->getPixelHeight() ) ).copyTo( objectMat );

                // Create a QPixmap
                QPixmap pixmap = QPixmap::fromImage( QtHelper::cvMatToQImage( objectMat ) );

                // Find filename that does not already exist
                QString objectName = QString::fromStdString( (*k)->getName() );

                QString fileExtension = "png";

                QString objectImageFileName = objectName + "." + fileExtension;

                QString objectImageFileNameWithPath = absolutePath + "/" + fileNameWithoutExtension + "/" + objectImageFileName;

                QFileInfo fileInfo( objectImageFileNameWithPath );

                int count = 0;

                while ( fileInfo.exists() ) {

                    objectImageFileName = objectName + "_" + QString::number( count ) + "." + fileExtension;
                    objectImageFileNameWithPath = absolutePath + "/" + fileNameWithoutExtension + "/" + objectImageFileName;
                    fileInfo.setFile( objectImageFileNameWithPath );
                    count++;
                }

                // Save pixmap
                pixmap.save( objectImageFileNameWithPath );


                if( isfileForHTMLopened )
                {
                    xmlWriter.writeStartElement("tr");

                    xmlWriter.writeStartElement("td");
                    xmlWriter.writeCharacters( objectName );
                    xmlWriter.writeEndElement();

                    QFileInfo fileInfo( QString::fromStdString((*i)->getFilename()) );
                    QString filenameWithoutPath = fileInfo.fileName();

                    xmlWriter.writeStartElement("td");
                    xmlWriter.writeCharacters( filenameWithoutPath );
                    xmlWriter.writeEndElement();

                    xmlWriter.writeStartElement("td");
                    xmlWriter.writeCharacters(  QString::fromStdString((*j)->getChannelName()) );
                    xmlWriter.writeEndElement();


                    Position * pos = (*k)->getPosition();

                    if(pos){
                        xmlWriter.writeStartElement("td");
                        xmlWriter.writeCharacters(  QString::number(pos->getLongitude(), 'f', 15) );
                        xmlWriter.writeEndElement();

                        xmlWriter.writeStartElement("td");
                        xmlWriter.writeCharacters(  QString::number(pos->getLatitude(), 'f', 15) );
                        xmlWriter.writeEndElement();
                    }
                    else{
                        xmlWriter.writeStartElement("td");
                        xmlWriter.writeCharacters( "N/A" );
                        xmlWriter.writeEndElement();

                        xmlWriter.writeStartElement("td");
                        xmlWriter.writeCharacters( "N/A" );
                        xmlWriter.writeEndElement();
                    }


                    if((*k)->getWidth() > 0){
                        xmlWriter.writeStartElement("td");
                        xmlWriter.writeCharacters( QString::number( (*k)->getWidth(), 'f', 3) );
                        xmlWriter.writeEndElement();
                    }
                    else{
                        xmlWriter.writeStartElement("td");
                        xmlWriter.writeCharacters( "N/A" );
                        xmlWriter.writeEndElement();
                    }


                    if((*k)->getHeight() > 0){
                        xmlWriter.writeStartElement("td");
                        xmlWriter.writeCharacters( QString::number( (*k)->getHeight(), 'f', 3) );
                        xmlWriter.writeEndElement();
                    }
                    else{
                        xmlWriter.writeStartElement("td");
                        xmlWriter.writeCharacters( "N/A" );
                        xmlWriter.writeEndElement();
                    }

                    xmlWriter.writeStartElement("td");

                    QString imageString = "img src=\"" + fileNameWithoutExtension + "/" + objectImageFileName + "\" alt=\"" + objectImageFileName + "\"";
                    xmlWriter.writeStartElement( imageString );
                    xmlWriter.writeEndElement(); // imageString

                    xmlWriter.writeEndElement(); // td

                    xmlWriter.writeEndElement(); // tr

                }

            }
        }
    }

    if( isfileForHTMLopened )
    {
//        xmlWriter.writeEndElement(); // table
//        xmlWriter.writeEndElement(); // body
//        xmlWriter.writeEndElement(); // html

        xmlWriter.writeEndDocument(); // Closes all remaining open start elements and writes a newline.

        file.close();
    }

}

void Project::createAndSaveTrainingObjectSamples( const QString & folder,
                                                  const ParameterscvCreateTrainingSamples & parameterscvCreateTrainingSamples )
{
    std::cout << "\nBeginning Project::createAndSaveTrainingObjectSamples()\n" << std::endl;


    // Build background images and bg.txt file

    QString fileNameBgDotTxt = folder + "/" + "bg.txt";

    std::ofstream outFile;
    outFile.open( fileNameBgDotTxt.toStdString(), std::ofstream::out );

    if ( outFile.is_open() == false ) {
        // TODO: warning dialog window
        std::cout << "\nBeginning Project::createAndSaveTrainingObjectSamples()\n"
                  << "Cannot open file for bg.txt" << std::endl;

        return;
    }


    // Find maximum height and width for positive sample objects

    int maxObjectWidth = 0;
    int maxObjectHeight = 0;

    // i is an iterator to a ( SidescanFile * )
    for(auto i = files.begin(); i != files.end(); ++i){

        // j is an iterator to a (SidescanImage* )
        for(auto j=(*i)->getImages().begin();j!=(*i)->getImages().end();j++){

            // k is an iterator to (GeoreferencedObject *)
            for(auto k=(*j)->getObjects().begin();k!=(*j)->getObjects().end();k++){

                if ( (*k)->getPixelWidth() > maxObjectWidth )
                    maxObjectWidth = (*k)->getPixelWidth();

                if ( (*k)->getPixelHeight() > maxObjectHeight )
                    maxObjectHeight = (*k)->getPixelHeight();
            }
        }
    }

    std::cout << "maxObjectWidth:  " << maxObjectWidth << "\n"
              << "maxObjectHeight: " << maxObjectHeight << "\n" << std::endl;


    // Go through each image and save images of background region large enough

    // i is an iterator to a ( SidescanFile * )
    for(auto i = files.begin(); i != files.end(); ++i){

        std::cout << "\nSidescanFile's filename: " << (*i)->getFilename() << "\n" << std::endl;

        // j is an iterator to a (SidescanImage* )
        for(auto j=(*i)->getImages().begin();j!=(*i)->getImages().end();j++){

            std::cout << "\n\n  SidescanImage's channelName: " << (*j)->getChannelName() << "\n" << std::endl;

            int imageOverallWidth = (*j)->getImage().cols;
            int imageOverallHeight = (*j)->getImage().rows;

            std::cout << "imageOverallWidth:  " << imageOverallWidth << "\n"
                      << "maxObjectWidth:           " << maxObjectWidth << std::endl;
            std::cout << "imageOverallHeight: " << imageOverallHeight << "\n"
                      << "maxObjectHeight:          " << maxObjectHeight << std::endl;


            if ( imageOverallWidth < maxObjectWidth ) {
                std::cout << "\nimageOverallWidth: " << imageOverallWidth << "\n"
                            << "maxObjectWidth:    " << maxObjectWidth << std::endl;
                continue;
            }

            if ( imageOverallHeight < maxObjectHeight) {
                std::cout << "\nimageOverallHeight: " << imageOverallHeight << "\n"
                            << "maxObjectHeight:    " << maxObjectHeight << std::endl;
                continue;
            }


            if ( (*j)->getObjects().size() == 0 ) {

                // no objects, save the entire image as background
                std::cout << "    Image as no objects, background would start at height 0 and end at " << imageOverallHeight - 1 << std::endl;

                saveBackgroundImage( *j, folder, outFile, 0, imageOverallHeight - 1 );

            } else {

                std::cout << "    Image DOES HAVE objects\n" << std::endl;

                std::vector< std::pair <int,int> > objectsVerticalPositions;

                computeObjectsVerticalOccupancy( *j, objectsVerticalPositions );

                std::cout << "    back from call to computeObjectsVerticalOccupancy()\n" << std::endl;


                int backgroundTop = 0;

                for ( int count = 0; count < objectsVerticalPositions.size(); count++ )
                {
                    int backgroundBottom = objectsVerticalPositions[ count ].first - 1;

                    if ( backgroundBottom - backgroundTop + 1 >= maxObjectHeight) {

                        std::cout << "    Image background would start at height " << backgroundTop
                                   << " and end at " << backgroundBottom << std::endl;

                        saveBackgroundImage( *j, folder, outFile, backgroundTop, backgroundBottom );
                    }

                    backgroundTop = objectsVerticalPositions[ count ].second + 1;

                }


                // Background from the last object to the bottom of the overall image

                if ( backgroundTop < imageOverallHeight - 1
                        && ( imageOverallHeight - 1 ) - backgroundTop + 1 >= maxObjectHeight ) {

                    std::cout << "    Image background would start at height " << backgroundTop
                              << " and end at " << imageOverallHeight << std::endl;

                    saveBackgroundImage( *j, folder, outFile, backgroundTop, imageOverallHeight - 1 );
                }



            }



        }

    }


    // Go through each object and use "opencv_createsamples"

    // i is an iterator to a ( SidescanFile * )
    for(auto i = files.begin(); i != files.end(); ++i){

        // j is an iterator to a (SidescanImage* )
        for(auto j=(*i)->getImages().begin();j!=(*i)->getImages().end();j++){

            // k is an iterator to (GeoreferencedObject *)
            for(auto k=(*j)->getObjects().begin();k!=(*j)->getObjects().end();k++){


                // Copy the part of the cv::Mat with the object into a new cv::Mat
                cv::Mat objectMat;
                (*j)->getImage()( cv::Rect( (*k)->getX(), (*k)->getY(), (*k)->getPixelWidth(), (*k)->getPixelHeight() ) ).copyTo( objectMat );

                // Create a QPixmap
                QPixmap pixmap = QPixmap::fromImage( QtHelper::cvMatToQImage( objectMat ) );

                // Find filename that does not already exist
                QString objectName = QString::fromStdString( (*k)->getName() );

                QString fileExtension = "png";

                QString objectImageFileName = objectName + "." + fileExtension;

                QString objectImageFileNameWithPath = folder + "/" + objectImageFileName;

                QFileInfo fileInfo( objectImageFileNameWithPath );

                int count = 0;

                while ( fileInfo.exists() ) {

                    objectImageFileName = objectName + "_" + QString::number( count ) + "." + fileExtension;
                    objectImageFileNameWithPath = folder + "/" + objectImageFileName;
                    fileInfo.setFile( objectImageFileNameWithPath );
                    count++;
                }

                // Save pixmap
                pixmap.save( objectImageFileNameWithPath );


                int showsamples = 0;
                int width;
                int height;

                cv::setRNGSeed( parameterscvCreateTrainingSamples.rngseed );


                if ( parameterscvCreateTrainingSamples.useOriginalObjectImageWidthAsBasis ) {
                    width = (*k)->getPixelWidth() + parameterscvCreateTrainingSamples.nbPixelsChangeFromObjectImageWidth;

                    if ( width <= 0 )
                        width = (*k)->getPixelWidth(); // TODO: display a warning?

                } else {
                    width = parameterscvCreateTrainingSamples.width;
                }

                if ( parameterscvCreateTrainingSamples.useOriginalObjectImageHeightAsBasis ) {
                    height = (*k)->getPixelHeight() + parameterscvCreateTrainingSamples.nbPixelsChangeFromObjectImageHeight;

                    if ( height <= 0 )
                        height = (*k)->getPixelHeight(); // TODO: display a warning?

                } else {
                    height = parameterscvCreateTrainingSamples.height;
                }



                std::string imagenameString = objectImageFileNameWithPath.toStdString();
                std::string bgfilenameString = std::string( folder.toStdString() + "/bg.txt" );
                std::string vecnameString = std::string( objectImageFileNameWithPath.toStdString() + ".vec" );

                const char * imagename = imagenameString.c_str();
                const char * bgfilename = bgfilenameString.c_str();
                const char * vecname = vecnameString.c_str();

                std::cout << "\n"
                          << "imagename: \"" << imagename << "\"\n"
                          << "bgfilename: \"" << bgfilename << "\"\n"
                          << "vecname: \"" << vecname << "\"\n" << std::endl;

//                std::cout << "\n  Before strcpy( imagename," << std::endl;


                std::cout << "\n  Before call to cvCreateTrainingSamples()" << std::endl;

                cvCreateTrainingSamples( vecname, imagename,
                                         parameterscvCreateTrainingSamples.bgcolor,
                                         parameterscvCreateTrainingSamples.bgthreshold, bgfilename,
                                         parameterscvCreateTrainingSamples.num,
                                         parameterscvCreateTrainingSamples.invert,
                                         parameterscvCreateTrainingSamples.maxintensitydev,
                                         parameterscvCreateTrainingSamples.maxxangle,
                                         parameterscvCreateTrainingSamples.maxyangle,
                                         parameterscvCreateTrainingSamples.maxzangle,
                                         showsamples, width, height );


                std::cout << "\n  After call to cvCreateTrainingSamples()\n" << std::endl;


            }
        }
    }





    std::cout << "\nEnd of Project::createAndSaveTrainingObjectSamples()\n" << std::endl;

}

void Project::computeObjectsVerticalOccupancy( SidescanImage * image, std::vector< std::pair <int,int> > & verticalPositions)
{

    std::multiset< std::pair<int,int>, compare> set;

    std::pair <int,int> beginEnd;

    // k is an iterator to (GeoreferencedObject *)
    for( auto k = image->getObjects().begin(); k != image->getObjects().end(); k++ ){

        beginEnd = std::make_pair( (*k)->getY(), (*k)->getY() + (*k)->getPixelHeight() -1 );
        set.insert( beginEnd );

    }

    // Display
    std::multiset< std::pair <int,int> >::iterator it;
    std::cout << "\nset contains:\n\n";
    for ( it = set.begin(); it != set.end(); ++it )
        std::cout << " " << it->first << ", " << it->second << "\n";

    std::cout << "\n" << std::endl;


    // Combine object overlaps

    verticalPositions.clear();

    int setSize = set.size();

    // TODO: check if the size of the set is zero
    // if ( setSize == 0 )
    // {

    // }


    int beginning = set.begin()->first;
    int end = set.begin()->second;

    it = set.begin();
    it++;

    for ( int count = 1; count < setSize; count++ )
    {
        // if ( end >= it->first )
        if ( end >= it->first - 1 ) // With integers, if one object ends at 100 and the next object starts at 101, assume they are continuous
        {
            end = it->second;
        }
        else
        {
            std::pair <int,int> newPair = std::make_pair( beginning, end );

            verticalPositions.push_back( newPair );

            beginning = it->first;
            end = it->second;
        }

        ++it;
    }

    std::pair <int,int> newPair = std::make_pair( beginning, end );
    verticalPositions.push_back( newPair );

    // Display
    std::vector< std::pair <int,int> >::iterator itVec;
    std::cout << "\nverticalPositions contains:\n\n";
    for ( itVec = verticalPositions.begin(); itVec != verticalPositions.end(); ++itVec )
        std::cout << " " << itVec->first << ", " << itVec->second << "\n";

    std::cout << "\n" << std::endl;


}



void Project::saveBackgroundImage( SidescanImage * image, const QString & folder, std::ofstream & outFile,
                                   int backgroundTop, int backgroundBottom )
{
    std::cout << "\nBeginning of Project::saveBackgroundImage()\n" << std::endl;

    int height = backgroundBottom - backgroundTop + 1;

    // Copy the part of the cv::Mat with the object into a new cv::Mat
    cv::Mat objectMat;
    image->getImage()( cv::Rect( 0, backgroundTop, image->getImage().cols, height ) ).copyTo( objectMat );

    // Create a QPixmap
    QPixmap pixmap = QPixmap::fromImage( QtHelper::cvMatToQImage( objectMat ) );

    // Find filename that does not already exist
    QString objectName = QString::fromStdString( "background" );

    QString fileExtension = "png";

    QString objectImageFileName = objectName + "." + fileExtension;

//    QString objectImageFileNameWithPath = absolutePath + "/" + fileNameWithoutExtension + "/" + objectImageFileName;

    QString objectImageFileNameWithPath = folder + "/" + objectImageFileName;

    QFileInfo fileInfo( objectImageFileNameWithPath );

    int count = 0;

    while ( fileInfo.exists() ) {

        objectImageFileName = objectName + "_" + QString::number( count ) + "." + fileExtension;
//        objectImageFileNameWithPath = absolutePath + "/" + fileNameWithoutExtension + "/" + objectImageFileName;
        objectImageFileNameWithPath = folder + "/" + objectImageFileName;

        fileInfo.setFile( objectImageFileNameWithPath );

        count++;
    }

    // Save pixmap
    pixmap.save( objectImageFileNameWithPath );


    if ( outFile.is_open() ) {
        // Image's path written in the file must be relative to outFile's location
        outFile << objectImageFileName.toStdString() << std::endl;
    }

    std::cout << "\nEnd of Project::saveBackgroundImage()\n" << std::endl;


}

bool Project::areThereFiles() const
{
    if ( files.size() != 0 )
        return true;
    else
        return false;
}
bool Project::areThereObjects() const
{
    if ( areThereFiles() == false )
        return false;

    bool thereAreObjects = false;

    auto i = files.begin(); // i is an iterator to a ( SidescanFile * )

    while ( thereAreObjects == false && i != files.end() ) {

        auto j=(*i)->getImages().begin(); // j is an iterator to a (SidescanImage* )

        while ( thereAreObjects == false && j!=(*i)->getImages().end() ) {

            if ( (*j)->getObjects().size() != 0 )
                thereAreObjects = true;

            ++j;
        }

        ++i;
    }

    return thereAreObjects;
}
