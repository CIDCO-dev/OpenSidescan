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

                QString fileName = folder + "/" + objectName + "." + fileExtension;

                QFileInfo fileInfo( fileName );

                int count = 0;

                while ( fileInfo.exists() ) {
                    fileName = folder + "/" + objectName + "_" + QString::number( count ) + "." + fileExtension;
                    fileInfo.setFile( fileName );
                    count++;
                }

                // Save pixmap
                pixmap.save( fileName );
            }
        }
    }

}
