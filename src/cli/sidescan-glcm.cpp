/*
* Copyright 2021 © Centre Interdisciplinaire de développement en Cartographie des Océans (CIDCO), Tous droits réservés
*/

/*
* \author Guillaume Labbe-Morissette
*/

#ifndef MAIN_CPP
#define MAIN_CPP

#include "../thirdParty/MBES-lib/src/datagrams/DatagramParserFactory.hpp"
#include <iostream>
#include <vector>
#include <string>
#include "opencv2/opencv.hpp"
#include <cmath>

/**Writes the usage information about the datagram-list*/

void printUsage(){
	std::cerr << "\n\
	NAME\n\n\
	sidescan-dump - Dumps sidescan data of an image file\n\n\
	SYNOPSIS\n \
	sidescan-dump file\n\n\
	DESCRIPTION\n\n \
	Copyright 2021 © Centre Interdisciplinaire de développement en Cartographie des Océans (CIDCO), All rights reserved" << std::endl;
	exit(1);
}

/*!
* \brief Datagram Printer class
*
* Extends DatagramEventHandler.
*/
class SidescanGlcmCategorizer : public DatagramEventHandler{
public:
    
	/**
	* Creates a SidescanDataDumper
	*/
    
	SidescanGlcmCategorizer(std::string & filename):filename(filename){

	}

	/**Destroys the SidescanDataDumper*/
	~SidescanGlcmCategorizer(){

	}

        void processSidescanData(SidescanPing * ping){
            
            if(channels.size() < ping->getChannelNumber()+1){
                std::vector<SidescanPing*> * u = new std::vector<SidescanPing*>();
                channels.push_back(u);
            }

            channels[ping->getChannelNumber()]->push_back(ping);
        }
        
        //windowSize must be odd
        
        void computeGlcm(cv::Mat &img, int windowSize){
        	if(windowSize % 2 == 0){
        		std::cerr<<"windowSize must be odd !\n";
        		return;
        	}
        	else{
		    	//for every pixel in image
		    	for(int col = windowSize/2; col<img.cols - (windowSize/2); col=col+(windowSize/2)){
		    		for (int row = windowSize/2; row<img.rows - (windowSize/2); row=row+(windowSize/2)){
		    			//compute glcm over window
		    			cv::Mat glcm(cv::Size(256,256), CV_64F, cv::Scalar(0)); // XXX:is 32 bit enough?
		    			int count = 0;
		    			
		    			
		    			//for pixel in window
		    			for(int windowCol = col-(windowSize/2) +1 ; windowCol<col+(windowSize/2) -1; windowCol++){
		    				for(int windowRow = row-(windowSize/2) +1 ; windowRow<row+(windowSize/2) -1; windowRow++){
								
		    					int value = img.at<uchar>(windowRow, windowCol, 0);
		    					int valueUp = img.at<uchar>(windowRow-1, windowCol, 0);
		    					int valueDown = img.at<uchar>(windowRow+1, windowCol, 0);
		    					int valueLeft = img.at<uchar>(windowRow, windowCol-1, 0);
		    					int valueRight = img.at<uchar>(windowRow, windowCol+1, 0);
		    					
		    					glcm.at<double>(valueUp, value, 0)++;
		    					glcm.at<double>(valueDown, value, 0)++;
		    					glcm.at<double>(valueLeft, value, 0)++;
		    					glcm.at<double>(valueRight, value, 0)++;
		    					count += 4;
		    					
		    				}
		    			}
		    			
		    			cv::Mat transposedGlcm(cv::Size(256,256), CV_64F, cv::Scalar(0));
		    			cv::transpose(glcm, transposedGlcm);
		    			cv::Mat normalizedGlcm = (glcm + transposedGlcm)/(count*2);
		    			
		    			double energy = 0;
		    			double contrast = 0;
		    			double homogeneity = 0;
		    			double entropy = 0;
		    			double correlation = 0;
		    			double shade = 0;
		    			double prominence = 0;
		    			double glcmMean = 0;
		    			double squaredVarianceIntensity = 0;
		    			for(int c = 0; c <normalizedGlcm.cols; c++){
		    				for(int r = 0; r<normalizedGlcm.rows; r++){
		    					double pij = normalizedGlcm.at<double>(r,c,0);
		    					energy += pij * pij;	
		    					contrast += (c-r)*(c-r)*pij;
		    					homogeneity += pij/(1.0+((c-r)*(c-r)));
		    					entropy += -(log(pij)*pij);
		    					double intensity = (double)img.at<uchar>(col,row,0);
		    					glcmMean += pij * intensity;
		    					squaredVarianceIntensity += pij*((intensity - glcmMean)*(intensity - glcmMean));
		    					correlation += pij*(((c-glcmMean)*(r-glcmMean))/squaredVarianceIntensity);
		    					
		    				}
		    			}	    			
		    		}
		    	}
		    	
		    	//std::cout<<glcm<<"\n";
        		return;
		    }

        }
        
        void generateImages(){
            std::cerr << "Generating images for " << channels.size() << " channels" << std::endl;

            for(unsigned int i=0;i<channels.size();i++){
                std::stringstream ss;
                
                ss << filename <<  "-classes-" << i << ".jpg";
                std::cerr << "Channel " << i << std::endl;
                
                cv::Mat img(channels[i]->size(),channels[i]->at(0)->getSamples().size(), CV_64F,cv::Scalar(0));
                
                std::cerr << "Rows: " << channels[i]->size() << " Cols: " << channels[i]->at(0)->getSamples().size() << std::endl;                 
                
                for(unsigned int j=0;j<channels[i]->size();j++){ //j indexes rows
                    for(unsigned int k=0;k<channels[i]->at(j)->getSamples().size();k++){ //k indexes cols 
                        img.at<double>(j, k, 0) = channels[i]->at(j)->getSamples().at(k);
                    }
                }
                
                cv::normalize(img,img,200000,0);
		        cv::Mat I;
		        img.convertTo(I, CV_8UC1);
		
                //post-process greyscale
                
                equalizeHist(I,I);
                fastNlMeansDenoising(I,I);  
              
				//TODO: make this a command-line parameter
				// blur(I,I,cv::Size(2,2));
                
                // output classes
                cv::Mat classes;
                computeGlcm(I, 31);
                
                //std::cout<<classes<<"\n";
				//imwrite(ss.str(), classes);
            }
        }
        
private:
    std::string filename;
    
    std::vector<  std::vector<SidescanPing * > * > channels;
};

/**
* Declares the parser depending on argument received
*
* @param argc number of argument
* @param argv value of the arguments
*/
int main (int argc , char ** argv ){
	DatagramParser *    parser      = NULL;

        #ifdef __GNU__
	setenv("TZ", "UTC", 1);
	#endif
	#ifdef _WIN32
	putenv("TZ");
	#endif

	if(argc != 2){
		printUsage();
	}

	std::string fileName(argv[1]);

	try{
		SidescanGlcmCategorizer  sidescan(fileName);

		std::cerr << "[+] Decoding " << fileName << std::endl;

		parser = DatagramParserFactory::build(fileName,sidescan);

		parser->parse(fileName);
                
                sidescan.generateImages();
	}
	catch(std::exception * e){
		std::cerr << "[-] Error while parsing " << fileName << ": " << e->what() << std::endl;
	}


	if(parser) delete parser;
	
	
}


#endif
