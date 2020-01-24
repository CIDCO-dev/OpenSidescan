
#include <stdlib.h>     /* srand, rand */


#include "qthelper.h"



QtHelper::QtHelper()
{

}


QImage  QtHelper::cvMatToQImage( const cv::Mat &inMat )
{
   switch ( inMat.type() )
   {
      // 8-bit, 4 channel
      case CV_8UC4:
      {
         std::cout << "\nQtHelper::cvMatToQImage(), CV_8UC4\n" << std::endl;

         QImage image( inMat.data,
                       inMat.cols, inMat.rows,
                       static_cast<int>(inMat.step),
                       QImage::Format_ARGB32 );

         return image;
      }

      // 8-bit, 3 channel
      case CV_8UC3:
      {
         std::cout << "\nQtHelper::cvMatToQImage(), CV_8UC3\n" << std::endl;

         QImage image( inMat.data,
                       inMat.cols, inMat.rows,
                       static_cast<int>(inMat.step),
                       QImage::Format_RGB888 );

         return image.rgbSwapped();
      }

      // 8-bit, 1 channel
      case CV_8UC1:
      {
#if QT_VERSION >= QT_VERSION_CHECK(5, 5, 0)

         std::cout << "\nQtHelper::cvMatToQImage(), CV_8UC1, if QT_VERSION >= QT_VERSION_CHECK(5, 5, 0)\n" << std::endl;

//         QImage image( inMat.data,
//                       inMat.cols, inMat.rows,
//                       static_cast<int>(inMat.step),
//                       QImage::Format_Grayscale8 );

//        image = image.convertToFormat(QImage::Format_Indexed8 );


         QImage image( inMat.data,
                       inMat.cols, inMat.rows,
                       static_cast<int>(inMat.step),
                       QImage::Format_Indexed8 );

//        std::cout << "\ninMat.cols:                   " << inMat.cols << "\n"
//                    << "inMat.rows:                   " << inMat.rows << "\n"
//                    << "static_cast<int>(inMat.step): " << static_cast<int>(inMat.step) << "\n" << std::endl;


//        srand (time(NULL)); // initialize random seed

         QVector<QRgb> table( 256 );
         for( int i = 0; i < 255; ++i )
         {
//             table[i] = qRgb( rand() % 256, rand() % 256, rand() % 256); // random
//             table[i] = qRgb( 255-i, i,  ( i + 128 ) % 256 );
//             table[i] = qRgb( i, i, i ); // Greyscale
//             table[i] = qRgb( i, i, 0 ); // yellow

             table[i] = qRgb( i, static_cast<int>( i * 200.0/256)  , static_cast<int>( i * 50/256) );

//             std::cout << std::hex << static_cast<unsigned int >( table[ i ] ) << std::dec << "\n";

         }

         // The found objects' bonding box is displayed with a white rectangle
         // The color white must be in the color table
         table[255] = qRgb( 255, 255, 255 );

         // The object names are displayed in black, Does black have to be in the color table?

         image.setColorTable(table);


#else

       std::cout << "\nQtHelper::cvMatToQImage(), CV_8UC1, NOT( if QT_VERSION >= QT_VERSION_CHECK(5, 5, 0) )\n" << std::endl;

         static QVector<QRgb>  sColorTable;

         // only create our color table the first time
         if ( sColorTable.isEmpty() )
         {
            sColorTab
                    //Display object width
                    le.resize( 256 );

            for ( int i = 0; i < 256; ++i )
            {
               sColorTable[i] = qRgb( i, i, i );
            }
         }

         QImage image( inMat.data,std::to_string(xDistance)std::to_string(xDistance)
                       inMat.cols, inMat.rows,
                       static_cast<int>(inMat.step),std::to_string(xDistance)
                       QImage::Format_Indexed8 );

         image.setColorTable( sColorTable );
#endif

         return image;
      }

      default:
         std::cerr << "Unknown image type" << std::endl;
         break;
   }

   return QImage();
}

QPixmap QtHelper::cvMatToQPixmap( const cv::Mat &inMat )
{
   return QPixmap::fromImage( cvMatToQImage( inMat ) );
}
