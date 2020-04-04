#include "opencvhelper.h"

#include "../thirdParty/MBES-lib/src/utils/Exception.hpp"

#include <sstream>



// Inspired from https://stackoverflow.com/questions/7531981/how-to-instantiate-a-static-vector-of-object
cv::Mat OpencvHelper::colorTable(256, 1, CV_8UC3);
bool OpencvHelper::helperVariable = buildColorTable();


void OpencvHelper::detectObjects(std::vector<InventoryObject*> & objects,SidescanImage & image,int fastThreshold,int fastType,bool fastNonMaxSuppression,double dbscanEpsilon,int dbscanMinimumPoints,int mserDelta,int mserMinimumArea,int mserMaximumArea,bool mergeOverlappingObjects){

    image.resetDisplayedImage();

    /*  Detect Features ----------------------------------- */
    std::vector<cv::KeyPoint> combinedKeypoints;

    double boundingBoxPadding = 20;

    /* FAST & MSER feature detection */

    cv::FAST(image.getImage(),combinedKeypoints,fastThreshold,fastNonMaxSuppression,fastType);

    cv::Ptr<cv::MSER> detector= cv::MSER::create(mserDelta,mserMinimumArea,mserMaximumArea,0.25,0.2,200,1.01,0.003,5);
    detector->detect(image.getImage(),combinedKeypoints);

    for(auto i = combinedKeypoints.begin();i!=combinedKeypoints.end();i++){
        if(
           i->pt.x - i->size < 0   ||   i->pt.x + i->size > image.getDisplayedImage().size().width
                ||
           i->pt.y - i->size < 0   ||   i->pt.y + i->size > image.getDisplayedImage().size().height
           ){
            combinedKeypoints.erase(i--); //inline removal
        }
    }

    image.setMicroFeatures(combinedKeypoints);


    /* Cluster ------------------------------------------ */
    DBSCAN clustering(dbscanEpsilon, dbscanMinimumPoints, combinedKeypoints);

    clustering.run();

    std::vector<std::vector<int>> clusters = clustering.getCluster();

    //std::cerr << clusters.size() << " clusters found" << std::endl;

    std::vector<cv::Rect> rois;

    for(unsigned int i=0;i<clusters.size();i++){
        std::vector<cv::Point> points;

        for(unsigned int j=0;j<clusters[i].size();j++){
            //colorizedClusters.push_back(cv::KeyPoint(combinedKeypoints[  clusters[i][j]  ]));

            double xWest = combinedKeypoints[  clusters[i][j]  ].pt.x - combinedKeypoints[  clusters[i][j]  ].size;
            double yWest = combinedKeypoints[  clusters[i][j]  ].pt.y;

            double xEast = combinedKeypoints[  clusters[i][j]  ].pt.x + combinedKeypoints[  clusters[i][j]  ].size;
            double yEast = combinedKeypoints[  clusters[i][j]  ].pt.y;

            double xSouth = combinedKeypoints[  clusters[i][j]  ].pt.x;
            double ySouth = combinedKeypoints[  clusters[i][j]  ].pt.y - combinedKeypoints[ clusters[i][j] ].size;

            double xNorth = combinedKeypoints[  clusters[i][j]  ].pt.x;
            double yNorth = combinedKeypoints[  clusters[i][j]  ].pt.y + combinedKeypoints[ clusters[i][j] ].size;

            //Avoid feature clusters on the edges
            if(
                isInsideImage(xWest,yWest,image.getDisplayedImage())
                    &&
                isInsideImage(xEast,yEast,image.getDisplayedImage())
                    &&
                isInsideImage(xSouth,ySouth,image.getDisplayedImage())
                    &&
                isInsideImage(xNorth,yNorth,image.getDisplayedImage())
               ){

                points.push_back(cv::Point(combinedKeypoints[  clusters[i][j]  ].pt));                      //Center

                points.push_back(cv::Point( xWest - boundingBoxPadding  ,   yWest ));                       //West
                points.push_back(cv::Point( xEast + boundingBoxPadding  ,   yEast ));                       //East
                points.push_back(cv::Point( xSouth                      ,   ySouth - boundingBoxPadding));  //South
                points.push_back(cv::Point( xNorth                      ,   yNorth + boundingBoxPadding));  //North
            }
        }

        cv::Rect roi = cv::boundingRect(points);

        int yCenter = floor(roi.y + roi.height/(double)2);

        //adjust to fit in image
        int y = roi.y;
        int x = roi.x;
        int height = roi.height;
        int width = roi.width;
        
        if(y < 0){
            y = 0;
        }

        if(y > image.getPings().size() -1){
            y=image.getPings().size()-1;
        }

        if(y + height > image.getPings().size() - 1){
            height = std::max((int)(image.getPings().size()-y-1),0);
        }

        //Adjust x and width
        if(x < 0){
            x = 0;
        }        

        if(x > image.getDisplayedImage().size().width -1){
            x = image.getDisplayedImage().size().width-1;
        }

        if( x + width > image.getDisplayedImage().size().width - 1){
            width = std::max((int)(image.getDisplayedImage().size().width-x-1),0);
        }
        
        cv::Rect object(x,y,width,height);
        rois.push_back(object);
    }

    // fuse overlapping bounding boxes if demanded
    if(mergeOverlappingObjects){
        mergeOverlapping(rois);
    }

    for(auto i=rois.begin();i!=rois.end();i++){
        try {
            InventoryObject * object = new InventoryObject(image,(*i).x,(*i).y,(*i).width,(*i).height);
            objects.push_back(object);
        }
        catch (Exception * error) {
            std::cerr << "OpencvHelper::detectObjects(): " << error->what() << "\nFilename: \"" << image.getFile().getFilename() << "\n" << std::endl;
        }
    }
}


// Conversion based on
// https://en.wikipedia.org/wiki/HSL_and_HSV#HSV_to_RGB
double OpencvHelper::fn( const double n, const double h360, const double saturation01, const double value01 ) {
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


void OpencvHelper::HSVtoRGB( const double h360,
               const double saturation01,
               const double value01,
               std::vector<unsigned char> & rgb ) {
    // Each of R, G, B, as computed by fn, is within [0, 1]

    if ( rgb.size() != 3 )
        rgb.resize( 3 );

    rgb[ 0 ] = static_cast<unsigned char>( fn( 5, h360, saturation01, value01 ) * 256 );
    rgb[ 1 ] = static_cast<unsigned char>( fn( 3, h360, saturation01, value01 ) * 256 );
    rgb[ 2 ] = static_cast<unsigned char>( fn( 1, h360, saturation01, value01 ) * 256 );
}


bool OpencvHelper::buildColorTable() {

    double hue360 = 39.069767441860456;
    double saturation = 0.9005235602094241;

    std::vector<unsigned char> rgb;

    for( int i = 0; i < 256; ++i )
    {
        HSVtoRGB( hue360, saturation, i / 256.0, rgb );

        // OpenCV uses BGR, not RGB
        colorTable.at<cv::Vec3b>(i,0).val[0] = rgb[ 2 ];
        colorTable.at<cv::Vec3b>(i,0).val[1] = rgb[ 1 ];
        colorTable.at<cv::Vec3b>(i,0).val[2] = rgb[ 0 ];
    }

    return true;
}

void OpencvHelper::draw(SidescanImage & img, bool showObjectBoundingBox,bool showObjectSize,bool showObjectCenter, bool showMicroFeatures){

    img.resetDisplayedImage();

    // Apply color map to displayed image, will change image from CV_8UC1 to CV_8UC3
    cv::applyColorMap(img.getDisplayedImage(), img.getDisplayedImage(), colorTable);


    if(showMicroFeatures) {
        cv::drawKeypoints(img.getDisplayedImage(),img.getMicroFeatures(),img.getDisplayedImage(), cv::Scalar(255,0,0),cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
    }

    for(auto i = img.getObjects().begin();i!=img.getObjects().end();i++){

        if(showObjectBoundingBox){
            cv::rectangle(img.getDisplayedImage(), cv::Rect((*i)->getX(),(*i)->getY(),(*i)->getPixelWidth(),(*i)->getPixelHeight()), cv::Scalar(255,255,255),2);
        }

        if(showObjectSize || showObjectCenter){
            //Display object width

            std::stringstream ss;

            if(showObjectSize){
                ss << (*i)->getWidth() << " m x " << (*i)->getHeight() << " m";
            }

            if(showObjectCenter){

                //Add a space if we have size in ss
                if(showObjectSize){
                    ss << " ";
                }

                ss << "(" << (*i)->getXCenter() << "," << (*i)->getYCenter() << ")";
            }

            int fontFace=cv::FONT_HERSHEY_SIMPLEX;
            double fontScale=0.5;
            int thickness = 2;
            int baseline;


            cv::Size sz = cv::getTextSize(ss.str(),fontFace,fontScale,thickness,&baseline);

            cv::Rect label((*i)->getX(),(*i)->getY()-thickness-sz.height-10,sz.width,sz.height+10);

            cv::rectangle(img.getDisplayedImage(), label, cv::Scalar(255,255,255),cv::FILLED);

            cv::putText(img.getDisplayedImage(), ss.str() ,cv::Point((*i)->getX(),(*i)->getY()-thickness-3),fontFace,fontScale,cv::Scalar(0,0,0),1,cv::LINE_4,false);
        }

    }
}

bool OpencvHelper::isInsideImage(double x,double y,cv::Mat & img){
    return
    x >= 0 && x <= img.size().width
            &&
    y >= 0 && y <= img.size().height;
}


bool OpencvHelper::isOverlapping(cv::Rect & r1,cv::Rect & r2){
        if(r1.x > r2.x+r2.width || r2.x > r1.x+r1.width ){
            return false; //one rectangle is on left of the other
        }

        if(r1.y + r1.height < r2.y || r2.y +r2.height < r1.y){
            return false;
        }

        return true;
}

void OpencvHelper::mergeOverlapping(std::vector<cv::Rect> & rects){
    bool foundOverlap = false;

    //do{
        loop:
        if(rects.size() > 1){
            for(auto i = rects.begin();i!= rects.end()-1;i++){
                for(auto j=i+1;j!=rects.end();j++){
                    if(i != j){
                        if(OpencvHelper::isOverlapping(*i,*j)){
                            i->x=std::min(i->x,j->x);
                            i->y=std::min(i->y,j->y);

                            int x2 = std::max(i->x+i->width,j->x+j->width);
                            int y2 = std::max(i->y+i->height,j->y+j->height);

                            i->width =x2-i->x;
                            i->height=y2-i->y;
                            rects.erase(j--);

                            foundOverlap = true;
                            goto loop; //Ave Satanas!
                        }
                    }
                }
            }
        }
   // }while(foundOverlap);

}
