#include "houghdetector.h"

HoughDetector::HoughDetector()
{

}

HoughDetector::~HoughDetector(){

}

void HoughDetector::detect(SidescanImage & image,std::vector<InventoryObject*> & objectsFound){
    image.resetDisplayedImage();

    //cv::Mat greyscale;
    //cv::cvtColor(image.getImage(), greyscale, cv::);

    cv::medianBlur(image.getImage(),image.getImage(),3);

    std::vector<cv::Vec3f> circles;
    cv::HoughCircles(image.getImage(), circles, cv::HOUGH_GRADIENT,
                    1, //inverse resolution ratio
                    10,  // minimum distance between circles
                    100, //param1
                    35,//35, //param2
                    10, //min radius
                    100 // max radius
            // (min_radius & max_radius) to detect larger circles
    );

    for( size_t i = 0; i < circles.size(); i++ )
    {
        cv::Vec3i c = circles[i];
        cv::Point center = cv::Point(c[0], c[1]);
        // circle center
        circle( image.getDisplayedImage(), center, 1, cv::Scalar(0,100,100), 3, cv::LINE_AA);
        // circle outline
        int radius = c[2];
        circle( image.getDisplayedImage(), center, radius, cv::Scalar(255,0,255), 3, cv::LINE_AA);

        InventoryObject * object = new InventoryObject(image,center.x,center.y,radius,radius);
        objectsFound.push_back(object);
    }
}
