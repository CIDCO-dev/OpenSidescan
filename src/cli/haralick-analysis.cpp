#include <iostream>
#include <cstddef>
//#include <fstream>
//#include <algorithm>
//#include <vector>

#include <mlpack/core.hpp>

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
	
	int gaussians = std::stoi(argv[2]); //TODO handle errors
	
	mlpack::gmm::GMM* gmm = NULL;
	gmm = new mlpack::gmm::GMM(std::size_t(gaussians), data.n_rows);
	
	double likelihood;
	const size_t maxIterations = 300;
	const double tolerance = 1e-10;
	const std::size_t kmeansMaxIterations = 1000;
	const std::size_t trials = 1;

	mlpack::gmm::EMFit<> em(maxIterations, tolerance, mlpack::kmeans::KMeans<>(kmeansMaxIterations));
	std::cerr<<"ok \n";
	likelihood = gmm->Train(std::move(data), 1, false, em);
	
	std::cout<< "Log-likelihood of estimate: " << likelihood << "." << std::endl;
	std::cout<< "nb gaussians: " << gmm->Gaussians() << "." << std::endl;
	
	return 0;
	
}

