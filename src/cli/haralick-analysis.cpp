#include <iostream>
#include <cstddef>
//#include <fstream>
//#include <algorithm>
//#include <vector>

#include <mlpack/core.hpp>
#include <limits> 

//#include <mlpack/prereqs.hpp>
//#include <mlpack/core/util/io.hpp>
//#include <mlpack/core/util/mlpack_main.hpp>

#include "mlpack/methods/gmm/gmm.hpp"
//#include "diagonal_gmm.hpp"
#include "mlpack/methods/gmm/no_constraint.hpp"
//#include "diagonal_constraint.hpp"

//#include <mlpack/methods/kmeans/refined_start.hpp>

int main (int argc , char ** argv ){
	if (argc < 3) {
		std::cerr << "usage: ./haralick-analysis <csv input filePath> <gaussians>" <<"\n";
		
		return -1;
	}
	
	
	arma::mat data;
	mlpack::data::Load(argv[1], data, true); //std::move(data)
	std::cerr<<"data loaded\n";
	
	int maxGaussians = std::stoi(argv[2]); //TODO handle errors
	int gaussian = 1;
	std::vector<double> metric{-std::numeric_limits<double>::max()};
	double likelihood;

	do{
	mlpack::gmm::GMM* gmm = NULL;
	gmm = new mlpack::gmm::GMM(std::size_t(gaussian), data.n_rows);
	
	
	const size_t maxIterations = 300;
	const double tolerance = 1e-10;
	const std::size_t kmeansMaxIterations = 1000;
	const std::size_t trials = 1;

	mlpack::gmm::EMFit<> em(maxIterations, tolerance, mlpack::kmeans::KMeans<>(kmeansMaxIterations));
	likelihood = gmm->Train(std::move(data), trials, false, em);
	
	std::cout<< "Log-likelihood of estimate: " << likelihood << "." << std::endl;
	metric.push_back(likelihood);
	gaussian++;
	}
	while(gaussian <= maxGaussians && likelihood > *(metric.end()-2));

	std::cout<<"optimal: "<< gaussian-1<< "\n";
	
	return 0;
	
}

