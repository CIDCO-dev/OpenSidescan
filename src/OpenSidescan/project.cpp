#include "project.h"

#include <QFile>
#include <QtXml>
#include <QPixmap>

#include <cstring>
#include "sidescanimager.h"
#include "qthelper.h"


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
                        SidescanImage * img = NULL;
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

                        if(!img){
                            //no image...wtf
                            std::cerr << "Malformed Project File: No image associated with object" << std::endl;
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
                    ss << (*k)->getPosition()->getLatitude() << "," << (*k)->getPosition()->getLongitude();

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

void Project::saveObjectImages( const QString & folder )
{
//    std::cout << "\nBeginning of Project::saveObjectImages()\n"
//        << "Folder: \"" << folder.toStdString() << "\"\n" << std::endl;


    // Open file, write beginning of the file

    QString fileNameHTML = folder + "/" + "description.html";

    std::ofstream outFile;
    outFile.open( fileNameHTML.toStdString(), std::ofstream::out );

    if( outFile.is_open() )
    {
        outFile << "<!DOCTYPE html>" << "\n";
        outFile << "<html>" << "\n";
        outFile << "<head>" << "\n";
        outFile << "<style>" << "\n";
        outFile << "table, th, td {" << "\n";
        outFile << "  border: 1px solid black;" << "\n";
        outFile << "  border-collapse: collapse;" << "\n";
        outFile << "}" << "\n";
        outFile << "th, td {" << "\n";
        outFile << "  padding: 5px;" << "\n";
        outFile << "}" << "\n";
        outFile << "th {" << "\n";
        outFile << "  text-align: left;" << "\n";
        outFile << "}" << "\n";
        outFile << "</style>" << "\n";
        outFile << "</head>" << "\n";
        outFile << "<body>" << "\n";

        outFile << "<h2>Left-align Headings</h2>" << "\n";
        outFile << "<p>To left-align the table headings, use the CSS text-align property.</p>" << "\n";

//        outFile << "<table style="width:100%">" << "\n";
        outFile << "<table style=\"width:100%\">" << "\n";
        outFile << "  <tr>" << "\n";
        outFile << "    <th>Name</th>" << "\n";
        outFile << "    <th>File</th>" << "\n";
        outFile << "    <th>Channel</th>" << "\n";
        outFile << "    <th>Longitude</th>" << "\n";
        outFile << "    <th>Latitude</th>" << "\n";
        outFile << "    <th>Width (m)</th>" << "\n";
        outFile << "    <th>Height (m)</th>" << "\n";
        outFile << "    <th>Image</th>" << "\n";
        outFile << "  </tr>" << "\n";
        outFile << "  <tr>" << "\n";
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


                if( outFile.is_open() )
                {
                    outFile << "  <tr>" << "\n";
                    outFile << "    <td>" << objectName.toStdString() << "</td>" << "\n";

                    QFileInfo fileInfo( QString::fromStdString((*i)->getFilename()) );
                    QString filenameWithoutPath = fileInfo.fileName();

                    outFile << "    <td>" << filenameWithoutPath.toStdString() << "</td>" << "\n";

                    outFile << "    <td>" << QString::fromStdString((*j)->getChannelName()).toStdString() << "</td>" << "\n";


                    outFile << std::setprecision(15);

                    Position * pos = (*k)->getPosition();

                    if(pos){
                        outFile << "    <td>" << pos->getLongitude() << "</td>" << "\n";
                        outFile << "    <td>" << pos->getLatitude() << "</td>" << "\n";
                    }
                    else{
                        outFile << "    <td>N/A</td>" << "\n";
                        outFile << "    <td>N/A</td>" << "\n";
                    }

                    outFile << std::setprecision(3);


                    if((*k)->getWidth() > 0){
                        outFile << "    <td>" << (*k)->getWidth()  << "</td>" << "\n";
                    }
                    else{
                        outFile << "    <td>N/A</td>" << "\n";
                    }


                    if((*k)->getHeight() > 0){
                        outFile << "    <td>" << (*k)->getHeight()  << "</td>" << "\n";
                    }
                    else{
                        outFile << "    <td>N/A</td>" << "\n";
                    }


                    outFile << "    <td><img src=\"" << objectImageFileName.toStdString() << "\" alt=\"" << objectImageFileName.toStdString()
                            << "\"></td>" << "\n";

                    outFile << "  </tr>" << std::endl;

                }

            }
        }
    }

    if( outFile.is_open() )
    {
        outFile << "</table>" << "\n";
        outFile << "</body>" << "\n";
        outFile << "</html>" << "\n";
    }





}
