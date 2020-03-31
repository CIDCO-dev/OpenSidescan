#include "project.h"

#include <QFile>
#include <QtXml>
#include <QPixmap>

#include <cstring>


//#include <set>
//#include <utility>      // std::pair, std::make_pair

#include <vector>

//#include "opencv2/core.hpp"
//#include "../thirdParty/opencv/apps/createsamples/utility.hpp"



#include "sidescanimager.h"
#include "qthelper.h"





Project::Project()
{
    //Empty project with (0,0,0) lever arm
    Eigen::Vector3d leverArm;
    leverArm << 0.0,0.0,0.0;
    this->setAntenna2TowPointLeverArm(leverArm);
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
                    currentFile = imager.generate(filename, antenna2TowPointLeverArm);

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

void Project::exportInventoryAsCsv(std::string & filename){

    // std::cout << "\nBeginning of Project::exportInventoryAsCsv(std::string & filename)\n" << std::endl;

    std::ofstream outFile;
    outFile.open( filename, std::ofstream::out );

    if( outFile.is_open() == false )
    {
        return;
    }

    outFile << "name" << "," << "description" << ","
        << "longitude" << "," << "latitude" << "\n";

    outFile << std::fixed << std::setprecision(15);

    // i is an iterator to a ( SidescanFile * )
    for(auto i = files.begin(); i != files.end(); ++i){

        // j is an iterator to a (SidescanImage* )
        for(auto j=(*i)->getImages().begin();j!=(*i)->getImages().end();j++){

            // k is an iterator to (GeoreferencedObject *)
            for(auto k=(*j)->getObjects().begin();k!=(*j)->getObjects().end();k++){

                Position * pos = (*k)->getPosition();

                // Using quotation marks to support strings with line changes
                // or comma
                outFile << "\"" << (*k)->getName() << "\","
                    << "\"" << (*k)->getDescription() << "\","
                    << pos->getLongitude() << ","
                    << pos->getLatitude() << "\n";
            }
        }
    }

    outFile.close();
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

//void Project::createAndSaveTrainingObjectSamples( const QString & folder,
//                                                  const ParameterscvCreateTrainingSamples & parameterscvCreateTrainingSamples )
//{
//    std::cout << "\nBeginning Project::createAndSaveTrainingObjectSamples()\n" << std::endl;


//    int numberOfObjects = computeNumberOfObjects();

//    if ( numberOfObjects == 0 )
//        return;



//    // Create folder structure

//    std::string originalObjectImages = "OriginalObjectImages";
//    std::string outputPositiveSamples = "OutputPositiveSamples";
//    std::string background = "Background";


//    QString folderOriginalObjectImages = folder + "/" + QObject::tr( originalObjectImages.c_str() );
//    QString folderOutputPositiveSamples = folder + "/" + QObject::tr( outputPositiveSamples.c_str() );
//    QString folderBackground = folder + "/" + QObject::tr( background.c_str() );







//    // Build background images and bg.txt file



//    QString fileNameBgDotTxt = folderBackground + "/" + "bg.txt";

//    std::ofstream outFile;
//    outFile.open( fileNameBgDotTxt.toStdString(), std::ofstream::out );

//    if ( outFile.is_open() == false ) {
//        // TODO: warning dialog window
//        std::cout << "\nBeginning Project::createAndSaveTrainingObjectSamples()\n"
//                  << "Cannot open file for bg.txt" << std::endl;

//        return;
//    }

//    // TODO TODO TODO


//    QProgressDialog progress("Finding objects...", QString(), 0, files.size(), this);

//    progress.setWindowModality(Qt::WindowModal);
//    progress.setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint | Qt::WindowTitleHint);

//    progress.setValue(0);
//    progress.setMinimumDuration( 0 );







//    std::cout << "\nEnd of Project::createAndSaveTrainingObjectSamples()\n" << std::endl;

//}







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

int Project::computeNumberOfObjects() const
{

    if ( areThereFiles() == false )
        return 0;

    int numberOfObjects = 0;

    // i is an iterator to a ( SidescanFile * )
    for(auto i = files.begin(); i != files.end(); ++i){

        // j is an iterator to a (SidescanImage* )
        for(auto j=(*i)->getImages().begin();j!=(*i)->getImages().end();j++){

            numberOfObjects += (*j)->getObjects().size();

        }
    }

    return numberOfObjects;
}

