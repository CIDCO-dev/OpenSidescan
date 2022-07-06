/*
* Copyright 2022 © Centre Interdisciplinaire de développement en Cartographie des Océans (CIDCO), Tous droits réservés
*/

/*
* \author Patrick Charron-Morneau
*/


#include <iostream>
#include <cstddef>
#include <mlpack/core.hpp>
#include <limits> 
#include "mlpack/methods/gmm/gmm.hpp"
#include "mlpack/methods/gmm/no_constraint.hpp"
#include "opencv2/opencv.hpp"

int findOptimalNbClusters(arma::mat features, int maxGaussians){ 

	int gaussian = 1;
	std::vector<double> metric{-std::numeric_limits<double>::max()};
	double likelihood;

	do{
		mlpack::gmm::GMM* gmm = NULL;
		gmm = new mlpack::gmm::GMM(std::size_t(gaussian), features.n_rows);
		
		
		const size_t maxIterations = 300;
		const double tolerance = 1e-10;
		const std::size_t kmeansMaxIterations = 1000;
		const std::size_t trials = 3;

		mlpack::gmm::EMFit<> em(maxIterations, tolerance, mlpack::kmeans::KMeans<>(kmeansMaxIterations));
		likelihood = gmm->Train(std::move(features), trials, false, em);
		
		std::cout<< "Log-likelihood of estimate: " << likelihood << "." << std::endl;
		metric.push_back(likelihood);
		gaussian++;
	}
	while(gaussian <= maxGaussians && likelihood > *(metric.end()-2));

	std::cout<<"optimal: "<< gaussian-1<< "\n";
	
	return gaussian-1;

}

int main (int argc , char ** argv ){
	if (argc < 3) {
		std::cerr << "usage: ./haralick-analysis <csv input filePath> <gaussians>" <<"\n";
		
		return -1;
	}
	
	int maxGaussians = 0;
	arma::mat data;
	mlpack::data::Load(argv[1], data, true); //std::move(data)
	std::cerr<<data.n_cols << " features loaded\n";
	std::cerr<<data.n_rows << " nb features \n";
	arma::mat features(data.submat(2, 0, 8, data.n_cols-1));
	
	
	try{
		maxGaussians = std::stoi(argv[2]);
	}
	catch(std::invalid_argument &err){
		std::cerr<<err.what()<<"\n";
	}
	
	int bestFit = findOptimalNbClusters(features, maxGaussians);
	
	arma::Row<size_t> labels;
	
	//int bestFit = 5;

	mlpack::gmm::GMM* gmm = NULL;
	gmm = new mlpack::gmm::GMM(std::size_t(bestFit), features.n_rows);
	
	const size_t maxIterations = 300;
	const double tolerance = 1e-10;
	const std::size_t kmeansMaxIterations = 1000;
	const std::size_t trials = 1;

	mlpack::gmm::EMFit<> em(maxIterations, tolerance, mlpack::kmeans::KMeans<>(kmeansMaxIterations));
	double likelihood = gmm->Train(std::move(features), trials, false, em);
	
	gmm->Classify(std::move(features), labels);
	
	

	
	cv::Mat image(2009, 1024, CV_8UC3, cv::Scalar(0));
	uchar colorMultiplier = 255/(bestFit);
	//uchar colorMultiplier = (255*3)/(bestFit);
	
	int windowSize = 31;
	
	for(int y = 0; y< labels.n_cols; y++){
		int windowMiddleY = data(0,y);
		int windowMiddleX = data(1,y);
		uchar colorClass = uchar((labels[y]+1) * colorMultiplier);
		
		if(colorClass <= colorMultiplier){
			//std::cout<<y<<" : "<<int(colorClass)<<"\n";
			image(cv::Range(windowMiddleY - windowSize/2, windowMiddleY + windowSize/2), 
			  cv::Range(windowMiddleX - windowSize/2, windowMiddleX + windowSize/2)).setTo(cv::Vec3b(colorClass,0,0));
		}
		if(colorClass <= colorMultiplier*2 && colorClass > colorMultiplier){
			//std::cout<<y<<" : "<<int(colorClass)<<"\n";
			image(cv::Range(windowMiddleY - windowSize/2, windowMiddleY + windowSize/2), 
			  cv::Range(windowMiddleX - windowSize/2, windowMiddleX + windowSize/2)).setTo(cv::Vec3b(0,colorClass,0));
		}
		if(colorClass > colorMultiplier*2){
			//std::cout<<y<<" : "<<int(colorClass)<<"\n";
			image(cv::Range(windowMiddleY - windowSize/2, windowMiddleY + windowSize/2), 
			  cv::Range(windowMiddleX - windowSize/2, windowMiddleX + windowSize/2)).setTo(cv::Vec3b(0,0,colorClass));
		}
		
	}
	/*
	cv::imshow("image", image);
	char c = (char)cv::waitKey(0);
	cv::destroyAllWindows();
	*/
	cv::imwrite("result.png", image);
	return 0;
	
}

