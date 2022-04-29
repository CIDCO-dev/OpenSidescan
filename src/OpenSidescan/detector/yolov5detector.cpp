#include "yolov5detector.h"

Yolov5Detector::Yolov5Detector()
{
    std::string modelPath = "/home/pat/projet/libtorch_imports/best2.onnx"; //TODO
    this->net = cv::dnn::readNetFromONNX(modelPath);
    //TODO load file with class name -> idealy parse yml file used for training
}


void Yolov5Detector::detect(SidescanImage & image,std::vector<InventoryObject*> & objectsFound){
    cv::Mat img = image.getImage();
    cv::Mat blob = cv::dnn::blobFromImage(img, 1.0/255, cv::Size(640,640), cv::Scalar(0,0,0),true, false);
    net.setInput(blob);
    cv::Mat output = net.forward();
    cv::Mat detections(output.size[1], output.size[2], CV_32F, output.ptr<float>());
    std::vector<cv::Rect> boxes;
    std::vector<int> indices;
    std::vector<float> scores;
    // add to constructor
    const float scoresThreshold = 0.5;
    const float nmsThreshold = 0.45;
    const float confidenceThreshold = 0.3;
    std::string className = "Crab trap ";

    for(int i =0; i < detections.rows; i++){

        float confidence = detections.at<float>(i,4);

        if(confidence > confidenceThreshold){

            cv::Mat guess = detections(cv::Range(i,i+1),cv::Range(5, detections.cols));
            double min, max;
            int *indexMax, *indexMin;
            cv::minMaxIdx(guess, &min, &max, indexMin, indexMax);
            int boxWidth = static_cast<int>(detections.at<float>(i, 2));
            int boxHeight = static_cast<int>(detections.at<float>(i, 3));
            float centerX = (detections.at<float>(i, 0) / 640.0) * img.size[1];
            float centerY = (detections.at<float>(i, 1) / 640.0) * img.size[0];
            int boxX = static_cast<int>(centerX - (boxWidth / 2));
            int boxY = static_cast<int>(centerY - (boxHeight / 2));
            cv::Rect box(boxX, boxY, boxWidth, boxHeight);
            boxes.push_back(box);
        }
    }
    cv::dnn::NMSBoxes(boxes, scores, scoresThreshold, nmsThreshold, indices);
    for (int i = 0; i < indices.size(); i++){

        int idx = indices[i];
        cv::Rect box = boxes[idx];
        int left = box.x;
        int top = box.y;
        int width = box.width;
        int height = box.height;

        InventoryObject * object = new InventoryObject(image, box.x, box.y, box.width, box.height, className+ std::to_string(int(100*scores[idx])) + "%");
        objectsFound.push_back(object);
    }
}
