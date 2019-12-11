/*
* Copyright 2019 © Centre Interdisciplinaire de développement en Cartographie des Océans (CIDCO), Tous droits réservés
*/

 /*
 * \author Christian Bouchard
 */


#include "workerimportsidescanfiles.h"

#include "sidescanimager.h"


WorkerImportSidescanFiles::WorkerImportSidescanFiles( QStringList & fileNames,
                           ProjectWindow * projectWindow,
                           SidescanFile ** lastFile)
    : fileNames( fileNames ), projectWindow( projectWindow ), lastFile( lastFile ),
//      success( false ),
//      didGetException( false ),
      exceptionText( "" )
{


}


//bool WorkerImportSidescanFiles::getSuccess() const
//{
//    return success;
//}

//bool WorkerImportSidescanFiles::getDidGetException() const
//{
//    return didGetException;
//}


std::string WorkerImportSidescanFiles::getExceptionString() const
{
    return exceptionText;
}




void WorkerImportSidescanFiles::doWork() {

    unsigned int nbFiles = fileNames.size();

    unsigned int operationCount=0;

    DatagramParser * parser = NULL;

    try {

        for(auto i=fileNames.begin();i!=fileNames.end();i++){

            std::string sFileName = (*i).toStdString();

            if(!projectWindow->containsFile(sFileName)){
                SidescanImager imager;

                parser = DatagramParserFactory::build(sFileName,imager);
                parser->parse(sFileName);

                progressInt(++operationCount);
                progressFilename(QString::fromStdString(sFileName));

                SidescanFile * file = imager.generate(sFileName);

                projectWindow->addFile(file);

                *lastFile = file;

                progressInt(++operationCount);

                delete parser;
                parser=NULL;
            }
            else{
                //Project already contains file. Ignore silently.
            }
        }


    }
    catch ( std::exception * e )
    {
//        didGetException = true;

        if ( parser != NULL )
            delete parser;

        exceptionText =  e->what();

        qDebug() << tr( "WorkerImportSidescanFiles exception" );

        // TODO: progressInt(2*nbFiles);     to close the progress dialog?
    }


    qDebug() << tr( "End of DoWork" );

    progressInt(2*nbFiles);


}
