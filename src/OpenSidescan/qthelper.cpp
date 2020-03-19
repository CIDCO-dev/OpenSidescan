#include <math.h>       /* fmod */


#include "qthelper.h"

// Conversion based on
// https://en.wikipedia.org/wiki/HSL_and_HSV#HSV_to_RGB
double fn( const double n,
           const double h360,
           const double saturation01,
           const double value01 )
{    
    // f(n) = V - V * S * max( min( k, 4-k, 1 ), 0 )

    double k = fmod( n + h360 / 60, 6.0 );

    double minimum = k;

    if ( 4 - k < minimum )
        minimum = 4 - k;

    if ( 1 < minimum )
        minimum = 1.0;

    double maximum;

    if ( minimum > 0 )
        maximum = minimum;
    else
        maximum = 0.0;

    return value01 - value01 * saturation01 * maximum;
}

void HSVtoRGB( const double h360,
               const double saturation01,
               const double value01,
               std::vector<int> & rgb )
{
    // Each of R, G, B, as computed by fn, is within [0, 1]

    if ( rgb.size() != 3 )
        rgb.resize( 3 );

    rgb[ 0 ] = static_cast<int>( fn( 5, h360, saturation01, value01 ) * 256 );
    rgb[ 1 ] = static_cast<int>( fn( 3, h360, saturation01, value01 ) * 256 );
    rgb[ 2 ] = static_cast<int>( fn( 1, h360, saturation01, value01 ) * 256 );
}

// Inspired from https://stackoverflow.com/questions/7531981/how-to-instantiate-a-static-vector-of-object
QVector<QRgb> QtHelper::colorTable;
bool QtHelper::helperVariable = buildColorTable();


QtHelper::QtHelper()
{

}

bool QtHelper::buildColorTable()
{
    colorTable.resize( 256 );
    colorTable.squeeze(); // Releases any memory not required to store the items

    double hue360 = 39.069767441860456;
    double saturation = 0.9005235602094241;

    std::vector<int> rgb;

    for( int i = 0; i < 254; ++i )
    {
        HSVtoRGB( hue360, saturation, i / 256.0, rgb );

        colorTable[i] = qRgb( rgb[ 0 ], rgb[ 1 ] , rgb[ 2 ] );
    }

    // The microfeature circle is displayed in blue
    // The color blue must be in the color table
    colorTable[254] = qRgb( 255, 0, 0 );

    // The found objects' bonding box is displayed with a white rectangle
    // The color white must be in the color table
    colorTable[255] = qRgb( 255, 255, 255 );

    return true;
}


QImage  QtHelper::cvMatToQImage( const cv::Mat &inMat )
{
   switch ( inMat.type() )
   {
      // 8-bit, 4 channel
      case CV_8UC4:
      {
//         std::cout << "\nQtHelper::cvMatToQImage(), CV_8UC4\n" << std::endl;

         QImage image( inMat.data,
                       inMat.cols, inMat.rows,
                       static_cast<int>(inMat.step),
                       QImage::Format_ARGB32 );

         return image;
      }

      // 8-bit, 3 channel
      case CV_8UC3:
      {
//         std::cout << "\nQtHelper::cvMatToQImage(), CV_8UC3\n" << std::endl;

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

//         std::cout << "\nQtHelper::cvMatToQImage(), CV_8UC1, if QT_VERSION >= QT_VERSION_CHECK(5, 5, 0)\n" << std::endl;

         QImage image( inMat.data,
                       inMat.cols, inMat.rows,
                       static_cast<int>(inMat.step),
                       QImage::Format_Indexed8 );

         image.setColorTable(colorTable);

#else

//       std::cout << "\nQtHelper::cvMatToQImage(), CV_8UC1, NOT( if QT_VERSION >= QT_VERSION_CHECK(5, 5, 0) )\n" << std::endl;

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
