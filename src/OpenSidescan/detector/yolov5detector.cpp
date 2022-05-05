#include "yolov5detector.h"
#include <QDebug>
#include <QString>
#include <QFileInfo>
#include <QMessageBox>

Yolov5Detector::Yolov5Detector(std::string modelPath , float scoresThresh, float nmsThresh, float confidenceThresh)
{
    //std::string modelPath = "/home/pat/projet/libtorch_imports/latest.onnx"; //TODO
    QString model(QString::fromStdString(modelPath));
    this->scoresThreshold = scoresThresh;
    this->nmsThreshold = nmsThresh;
    this->confidenceThreshold = confidenceThresh;
    if(QFileInfo::exists(model)){
        try{
            this->net = cv::dnn::readNetFromONNX(modelPath);
        }
        catch(cv::Exception &e){
            QString errorMessage = e.what();
            QMessageBox msgBox;
            msgBox.setText(errorMessage);
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.exec();
        }
    }
    else{
        QMessageBox msgBox;
        msgBox.setText("model not found");
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
    }
    //TODO load file with class name -> idealy parse yml file used for training
}

void Yolov5Detector::detect(SidescanImage & image,std::vector<InventoryObject*> & objectsFound){

    cv::Mat gray = image.getImage();
    cv::Mat color;
    cv::cvtColor(gray, color, cv::COLOR_GRAY2BGR);
    for(int x = 0; x < color.cols; x +=640){
        int x2 = x + 640;
        if (x2 > color.cols){
            x2 = color.cols;
        }
        for(int y = 0; y < color.rows; y+=640){
            int y2 = y+640;
            if (y2 > color.rows){
                y2 = color.rows;
            }
            cv::Mat img = color(cv::Range(y, y2), cv::Range(x, x2));
            cv::Mat blob = cv::dnn::blobFromImage(img, 1.0/255, cv::Size(640,640), cv::Scalar(0,0,0),true, false);
            this->net.setInput(blob);

            cv::Mat output = net.forward();
            cv::Mat detections(output.size[1], output.size[2], CV_32F, output.ptr<float>());
            std::vector<cv::Rect> boxes;
            std::vector<int> indices;
            std::vector<float> scores;
            //TODO

            std::string className = "Crab trap "; //XXX

            for(int i =0; i < detections.rows; i++){

                float confidence = detections.at<float>(i,4);

                if(confidence > this->confidenceThreshold){

                    cv::Mat guess = detections(cv::Range(i,i+1),cv::Range(5, detections.cols));
                    double min, max;
                    cv::minMaxIdx(guess, &min, &max);
                    int boxWidth = static_cast<int>(detections.at<float>(i, 2));
                    int boxHeight = static_cast<int>(detections.at<float>(i, 3));
                    float centerX = (detections.at<float>(i, 0) / 640.0) * img.size[1];
                    float centerY = (detections.at<float>(i, 1) / 640.0) * img.size[0];
                    int boxX = static_cast<int>(centerX - (boxWidth / 2));
                    int boxY = static_cast<int>(centerY - (boxHeight / 2));
                    cv::Rect box(boxX +x, boxY+y, boxWidth, boxHeight);
                    boxes.push_back(box);
                    scores.push_back(max);

                }
            }
            cv::dnn::NMSBoxes(boxes, scores, this->scoresThreshold, this->nmsThreshold, indices);
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
    }
}
