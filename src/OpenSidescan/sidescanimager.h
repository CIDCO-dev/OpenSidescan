#ifndef SIDESCANIMAGER_H
#define SIDESCANIMAGER_H

#include "../thirdParty/MBES-lib/src/datagrams/DatagramParserFactory.hpp"
#include "../thirdParty/MBES-lib/src/math/Interpolation.hpp"
#include <iostream>
#include <vector>
#include <string>
#include "opencv2/opencv.hpp"
#include "sidescanfile.h"

/**
 * @brief The SidescanImager class
 *
 * Creates images out of sidescan data
 */
class SidescanImager : public DatagramEventHandler
{
public:

    SidescanImager(){

    }

    ~SidescanImager(){

    }

    void processFileProperties(std::map<std::string,std::string> * properties){
        fileInfo = properties;
    }

    void processChannelProperties(unsigned int channelNumber,std::string channelName,std::map<std::string,std::string> * properties){
        if(channelName.empty()){
            std::stringstream ss;
            ss << "Channel " << channelNumber;
            channelName = ss.str();
        }

        channelNames.push_back(channelName);
        channelInfo.push_back(properties);
    }

    void processPosition(uint64_t microEpoch,double longitude,double latitude,double height){
        positions.push_back(Position(microEpoch,latitude,longitude,height));
    }

    void processSidescanData(SidescanPing * ping){

        if(channels.size() < ping->getChannelNumber()+1){
            std::vector<SidescanPing*> * u = new std::vector<SidescanPing*>();
            channels.push_back(u);
        }

        channels[ping->getChannelNumber()]->push_back(ping);
    }

    SidescanFile * generate(std::string filename){
        SidescanFile * file = new SidescanFile(filename);
        std::vector<SidescanImage *> * matrixes = new std::vector<SidescanImage *>();

        //std::cerr << "Generating images for " << channels.size() << " channels" << std::endl;

        double averageXDistancePerPixel=0;

        for(unsigned int i=0;i<channels.size();i++){
            //std::cerr << "Channel " << i << std::endl;

            cv::Mat img(channels[i]->size(),channels[i]->at(0)->getSamples().size(), CV_64F,cv::Scalar(0));

            //std::cerr << "Rows: " << channels[i]->size() << " Cols: " << channels[i]->at(0)->getSamples().size() << std::endl;

            unsigned int positionIndex =0 ;

            for(unsigned int j=0;j<channels[i]->size();j++){ //j indexes rows

                //Grab position data IF AVAILABLE
                if(channels[i]->at(j)->getPosition() != NULL && ! positions.empty()){
                    //find position for ping
                    while(positionIndex + 1 < positions.size() && positions[positionIndex+1].getTimestamp() < channels[i]->at(j)->getTimestamp() ){
                        positionIndex++;
                    }

                    if(positionIndex >= positions.size() -1){
                        std::cerr << "No position data for ping" << std::endl;
                        break;
                    }

                    if(positions[positionIndex].getTimestamp() > channels[i]->at(j)->getTimestamp()){
                        std::cerr << "rejecting ping " << channels[i]->at(j)->getTimestamp() << " " << positions[positionIndex].getTimestamp() << std::endl;
                        continue;
                    }

                    Position* pingPosition = Interpolator::interpolatePosition(positions[positionIndex], positions[positionIndex+1], channels[i]->at(j)->getTimestamp());

                    channels[i]->at(j)->setPosition(pingPosition);
                }

                //generate pixel row
                for(unsigned int k=0;k<channels[i]->at(j)->getSamples().size();k++){ //k indexes cols
                    img.at<double>(j, k, 0) = channels[i]->at(j)->getSamples().at(k) ;
                }

                averageXDistancePerPixel += channels[i]->at(j)->getDistancePerSample();
            }

            averageXDistancePerPixel /= (double) channels[i]->size();

            cv::normalize(img,img,200000,0);
            cv::Mat * I = new cv::Mat();
            img.convertTo(*I, CV_8UC1);

            //post-process greyscale

            equalizeHist(*I,*I);

            fastNlMeansDenoising(*I,*I);
            blur(*I,*I,cv::Size(2,2));

            std::string channelName;

            if(i < channelNames.size()){
                channelName=channelNames[i];
            }
            else{
                std::stringstream ss;
                ss << "Channel " << i;
                channelName = ss.str();
            }

            std::map<std::string,std::string>  * properties = NULL;

            if(i < channelInfo.size()){
                properties=channelInfo[i];
            }

            matrixes->push_back(new SidescanImage(I,channels[i],i,channelName,properties));
            //imwrite(ss.str(), I);
        }

        file->setAverageXDistancePerPixel(averageXDistancePerPixel);
        file->setImages(matrixes);
        file->setFileProperties(fileInfo);
        fileInfo = NULL;

        return file;
    }

private:
    std::vector<Position> positions;
    std::vector<  std::vector<SidescanPing * > * > channels;
    std::map<std::string,std::string> * fileInfo = NULL;
    std::vector<  std::map<std::string,std::string>  *  >  channelInfo;
    std::vector< std::string > channelNames;
};

#endif // SIDESCANIMAGER_H
