#include "roidetector.h"

RoiDetector::RoiDetector(int fastThreshold,cv::FastFeatureDetector::DetectorType fastType,bool fastNonMaxSuppression,double dbscanEpsilon,int dbscanMinimumPoints,int mserDelta,int mserMinimumArea,int mserMaximumArea,bool mergeOverlappingObjects):fastThreshold(fastThreshold),fastType(fastType),fastNonMaxSuppression(fastNonMaxSuppression),dbscanEpsilon(dbscanEpsilon),dbscanMinimumPoints(dbscanMinimumPoints),mserDelta(mserDelta),mserMinimumArea(mserMinimumArea),mserMaximumArea(mserMaximumArea),mergeOverlappingObjects(mergeOverlappingObjects)
{

}

cv::FastFeatureDetector::DetectorType RoiDetector::toCvFastType (int fastType){

    switch(fastType){
    case 0:
        return cv::FastFeatureDetector::TYPE_5_8;
        break;
    case 1:
        return cv::FastFeatureDetector::TYPE_7_12;
        break;
    case 2:
        return cv::FastFeatureDetector::TYPE_9_16;
        break;
    default:
        return cv::FastFeatureDetector::TYPE_9_16;
    }
}

void RoiDetector::detect(SidescanImage & image,std::vector<InventoryObject*> & objectsFound){
	std::cout<<"roi detector debug mesage" <<std::endl;
    image.resetDisplayedImage();
	std::cout<<"roi detector debug mesage 1" <<std::endl;
    /*  Detect Features ----------------------------------- */
    std::vector<cv::KeyPoint> combinedKeypoints;

    double boundingBoxPadding = 20;

    /* FAST & MSER feature detection */

    cv::FAST(image.getImage(),combinedKeypoints,fastThreshold,fastNonMaxSuppression,fastType);
	std::cout<<"roi detector debug mesage 2" <<std::endl;
    cv::Ptr<cv::MSER> detector= cv::MSER::create(mserDelta,mserMinimumArea,mserMaximumArea,0.25,0.2,200,1.01,0.003,5);
    std::cout<<"roi detector debug mesage 3" <<std::endl;
	detector->detect(image.getImage(),combinedKeypoints);
	std::cout<<"roi detector debug mesage 4" <<std::endl;
    for(auto i = combinedKeypoints.begin();i!=combinedKeypoints.end();i++){
        if(
           i->pt.x - i->size < 0   ||   i->pt.x + i->size > image.getDisplayedImage().size().width
                ||
           i->pt.y - i->size < 0   ||   i->pt.y + i->size > image.getDisplayedImage().size().height
           ){
            combinedKeypoints.erase(i--); //inline removal
        }
    }
	std::cout<<"roi detector debug mesage 5" <<std::endl;
    image.setMicroFeatures(combinedKeypoints);
	std::cout<<"roi detector debug mesage 6" <<std::endl;

    /* Cluster ------------------------------------------ */
    DBSCAN clustering(dbscanEpsilon, dbscanMinimumPoints, combinedKeypoints);
	std::cout<<"roi detector debug mesage 7" <<std::endl;
    clustering.run();
	std::cout<<"roi detector debug mesage 8" <<std::endl;
    std::vector<std::vector<int>> clusters = clustering.getCluster();
	
    //std::cerr << clusters.size() << " clusters found" << std::endl;

    std::vector<cv::Rect> rois;
	std::cout<<"roi detector debug mesage 9" <<std::endl;
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
                OpencvHelper::isInsideImage(xWest,yWest,image.getDisplayedImage())
                    &&
                OpencvHelper::isInsideImage(xEast,yEast,image.getDisplayedImage())
                    &&
                OpencvHelper::isInsideImage(xSouth,ySouth,image.getDisplayedImage())
                    &&
                OpencvHelper::isInsideImage(xNorth,yNorth,image.getDisplayedImage())
               ){

                points.push_back(cv::Point(combinedKeypoints[  clusters[i][j]  ].pt));                      //Center

                points.push_back(cv::Point( xWest - boundingBoxPadding  ,   yWest ));                       //West
                points.push_back(cv::Point( xEast + boundingBoxPadding  ,   yEast ));                       //East
                points.push_back(cv::Point( xSouth                      ,   ySouth - boundingBoxPadding));  //South
                points.push_back(cv::Point( xNorth                      ,   yNorth + boundingBoxPadding));  //North
            }
        }
		std::cout<<"roi detector debug mesage 10" <<std::endl;
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
	std::cout<<"roi detector debug mesage 11" <<std::endl;

    // fuse overlapping bounding boxes if demanded
    if(mergeOverlappingObjects){
        OpencvHelper::mergeOverlapping(rois);
    }
	std::cout<<"roi detector debug mesage 12" <<std::endl;
    for(auto i=rois.begin();i!=rois.end();i++){
        InventoryObject * object = new InventoryObject(image,(*i).x,(*i).y,(*i).width,(*i).height);
        objectsFound.push_back(object);
    }
	std::cout<<"roi detector debug mesage 13" <<std::endl;
}
