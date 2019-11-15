#include "opencvhelper.h"

#include <sstream>



void OpencvHelper::detectObjects(std::vector<GeoreferencedObject*> & objects,SidescanFile & file,SidescanImage & image,int fastThreshold,int fastType,bool fastNonMaxSuppression,double dbscanEpsilon,int dbscanMinimumPoints,int mserDelta,int mserMinimumArea,int mserMaximumArea,bool showFeatureMarkers,bool mergeOverlappingObjects){

    image.resetDisplayedImage();

    /*  Detect Features ----------------------------------- */
    std::vector<cv::KeyPoint> combinedKeypoints;

    double boundingBoxPadding = 20;

    /* FAST feature detection, [Rosten06] */

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

    if(showFeatureMarkers){
        cv::drawKeypoints(image.getDisplayedImage(),combinedKeypoints,image.getDisplayedImage(),cv::Scalar(0,0,255),cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
    }

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
        
        //TODO: adjust width
        //std::cerr << x << "," << y << " (" << width << " x " << height << ")" << std::endl; 
        
        cv::Rect object(x,y,roi.width,height);
        rois.push_back(object);
    }

    //TODO: fuse overlapping bounding boxes
    if(mergeOverlappingObjects){
        mergeOverlapping(rois);
    }

    for(auto i=rois.begin();i!=rois.end();i++){
        GeoreferencedObject * object = new GeoreferencedObject(file,image,(*i).x,(*i).y,(*i).width,(*i).height);
        objects.push_back(object);
    }
}

void OpencvHelper::draw(SidescanImage & img, bool showObjectBoundingBox,bool showObjectSize,bool showObjectCenter){
    img.resetDisplayedImage();

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
