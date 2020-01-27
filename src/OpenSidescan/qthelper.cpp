
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


         int tableInt[ 256 ][ 3 ] = {

             { 0, 0, 0},
             { 5, 0, 6},
             { 8, 14, 2},
             { 13, 16, 5},
             { 15, 0, 12},
             { 15, 15, 3},
             { 17, 11, 0},
             { 18, 19, 11},
             { 20, 16, 17},
             { 21, 5, 15},
             { 22, 28, 2},
             { 24, 0, 2},
             { 25, 3, 6},
             { 25, 19, 0},
             { 27, 1, 0},
             { 27, 16, 20},
             { 28, 0, 6},
             { 29, 0, 5},
             { 31, 42, 0},
             { 35, 19, 4},
             { 38, 26, 10},
             { 39, 0, 27},
             { 39, 38, 10},
             { 47, 31, 8},
             { 48, 23, 18},
             { 48, 40, 3},
             { 49, 16, 7},
             { 53, 20, 13},
             { 53, 37, 3},
             { 55, 12, 32},
             { 56, 13, 0},
             { 56, 31, 34},
             { 57, 16, 10},
             { 57, 34, 0},
             { 61, 10, 9},
             { 63, 37, 0},
             { 66, 48, 10},
             { 67, 33, 21},
             { 69, 31, 22},
             { 69, 46, 4},
             { 71, 28, 12},
             { 72, 52, 15},
             { 73, 36, 17},
             { 74, 32, 20},
             { 74, 59, 40},
             { 75, 43, 20},
             { 79, 29, 6},
             { 79, 59, 6},
             { 80, 56, 8},
             { 82, 47, 19},
             { 82, 75, 33},
             { 84, 62, 12},
             { 84, 78, 2},
             { 85, 57, 56},
             { 86, 36, 37},
             { 86, 64, 4},
             { 88, 52, 30},
             { 89, 70, 11},
             { 90, 54, 28},
             { 90, 65, 45},
             { 93, 66, 23},
             { 93, 78, 11},
             { 97, 37, 3},
             { 99, 50, 17},
             { 100, 76, 50},
             { 101, 78, 10},
             { 102, 56, 22},
             { 102, 68, 7},
             { 103, 68, 10},
             { 103, 91, 39},
             { 104, 78, 29},
             { 105, 57, 37},
             { 106, 56, 5},
             { 106, 89, 45},
             { 107, 68, 27},
             { 109, 64, 31},
             { 111, 59, 1},
             { 111, 92, 24},
             { 112, 71, 5},
             { 113, 69, 4},
             { 113, 113, 59},
             { 115, 77, 15},
             { 115, 86, 26},
             { 116, 76, 4},
             { 117, 71, 11},
             { 117, 81, 5},
             { 121, 92, 12},
             { 122, 77, 18},
             { 122, 95, 48},
             { 122, 113, 22},
             { 123, 72, 6},
             { 124, 86, 37},
             { 124, 108, 49},
             { 126, 60, 2},
             { 126, 87, 30},
             { 126, 108, 0},
             { 132, 90, 32},
             { 132, 99, 2},
             { 132, 109, 13},
             { 133, 103, 5},
             { 134, 127, 85},
             { 135, 94, 2},
             { 136, 121, 62},
             { 137, 109, 9},
             { 139, 100, 7},
             { 141, 95, 0},
             { 143, 73, 3},
             { 143, 99, 26},
             { 144, 101, 9},
             { 146, 84, 11},
             { 148, 138, 23},
             { 149, 93, 16},
             { 150, 118, 19},
             { 151, 95, 18},
             { 153, 107, 30},
             { 154, 82, 8},
             { 156, 111, 30},
             { 157, 93, 29},
             { 157, 130, 25},
             { 158, 93, 25},
             { 158, 105, 0},
             { 159, 95, 0},
             { 159, 114, 29},
             { 160, 114, 3},
             { 160, 121, 54},
             { 161, 114, 34},
             { 161, 131, 33},
             { 162, 113, 46},
             { 164, 120, 49},
             { 164, 128, 40},
             { 166, 104, 29},
             { 166, 130, 10},
             { 168, 105, 0},
             { 168, 130, 21},
             { 168, 142, 23},
             { 170, 122, 24},
             { 171, 105, 29},
             { 171, 131, 17},
             { 171, 141, 17},
             { 172, 124, 39},
             { 172, 131, 52},
             { 174, 117, 10},
             { 175, 117, 18},
             { 175, 125, 12},
             { 176, 116, 4},
             { 177, 132, 7},
             { 179, 113, 27},
             { 179, 149, 17},
             { 180, 130, 41},
             { 181, 109, 0},
             { 181, 129, 20},
             { 184, 159, 43},
             { 186, 127, 61},
             { 186, 153, 48},
             { 187, 134, 30},
             { 187, 141, 30},
             { 187, 192, 92},
             { 188, 149, 30},
             { 189, 133, 36},
             { 190, 131, 53},
             { 190, 139, 30},
             { 190, 157, 42},
             { 191, 130, 5},
             { 191, 137, 41},
             { 191, 144, 72},
             { 193, 128, 34},
             { 193, 136, 69},
             { 193, 171, 51},
             { 195, 153, 19},
             { 196, 152, 47},
             { 197, 145, 7},
             { 198, 126, 44},
             { 198, 158, 37},
             { 202, 157, 66},
             { 202, 169, 14},
             { 203, 133, 35},
             { 207, 140, 35},
             { 208, 155, 53},
             { 209, 155, 47},
             { 210, 147, 34},
             { 211, 135, 59},
             { 212, 161, 46},
             { 212, 187, 105},
             { 213, 169, 44},
             { 214, 147, 7},
             { 215, 180, 88},
             { 216, 162, 40},
             { 216, 170, 23},
             { 216, 185, 58},
             { 217, 180, 89},
             { 218, 177, 25},
             { 218, 194, 24},
             { 219, 160, 24},
             { 219, 186, 73},
             { 220, 188, 115},
             { 221, 187, 54},
             { 223, 184, 57},
             { 224, 159, 69},
             { 225, 159, 63},
             { 225, 167, 31},
             { 226, 204, 131},
             { 227, 169, 33},
             { 227, 184, 80},
             { 228, 160, 87},
             { 228, 169, 33},
             { 228, 184, 77},
             { 229, 173, 148},
             { 229, 190, 85},
             { 231, 171, 25},
             { 232, 176, 63},
             { 232, 183, 65},
             { 232, 192, 120},
             { 233, 188, 45},
             { 234, 186, 84},
             { 235, 177, 41},
             { 235, 203, 94},
             { 236, 181, 101},
             { 237, 187, 92},
             { 237, 215, 96},
             { 240, 195, 68},
             { 241, 176, 34},
             { 242, 167, 50},
             { 242, 180, 33},
             { 242, 193, 91},
             { 243, 185, 16},
             { 243, 191, 115},
             { 244, 181, 104},
             { 244, 204, 47},
             { 245, 197, 69},
             { 245, 206, 77},
             { 246, 172, 49},
             { 246, 186, 40},
             { 247, 195, 59},
             { 249, 168, 16},
             { 249, 187, 26},
             { 249, 194, 77},
             { 249, 209, 87},
             { 251, 192, 74},
             { 251, 207, 142},
             { 252, 167, 61},
             { 253, 213, 177},
             { 253, 220, 55},
             { 254, 193, 50},
             { 254, 205, 87},
             { 254, 210, 113},
             { 254, 219, 127},
             { 254, 255, 215},
             { 255, 186, 109},
             { 255, 193, 75},
             { 255, 197, 67},
             { 255, 226, 116},
             { 255, 228, 119},
             { 255, 234, 127},
             { 255, 240, 214},
             { 255, 245, 183}

         };




         QVector<QRgb> table( 256 );
         for( int i = 0; i < 255; ++i )
         {
//             table[i] = qRgb( rand() % 256, rand() % 256, rand() % 256); // random
//             table[i] = qRgb( 255-i, i,  ( i + 128 ) % 256 );
//             table[i] = qRgb( i, i, i ); // Greyscale
//             table[i] = qRgb( i, i, 0 ); // yellow

             table[i] = qRgb( i, static_cast<int>( i * 200.0/256)  , static_cast<int>( i * 50/256) );

//             table[i] = qRgb( tableInt[ i ][ 0 ], tableInt[ i ][ 1 ] , tableInt[ i ][ 2 ] );




//             std::cout << std::hex << static_cast<unsigned int >( table[ i ] ) << std::dec << "\n";

         }

         // The found objects' bonding box is displayed with a white rectangle
         // The color white must be in the color table
         table[255] = qRgb( 255, 255, 255 );

         // The object names are displayed in black, Does black have to be in the color table?

//         image.setColorTable(table);


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
