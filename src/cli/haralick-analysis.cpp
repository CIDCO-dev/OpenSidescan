#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <mlpack/core.hpp>
#include <mlpack/methods/neighbor_search/neighbor_search.hpp>
#include <mlpack/methods/neighbor_search/ns_model.hpp>
/*
int main (int argc , char ** argv ){
	if (argc < 2) {
		std::cerr << "usage: ./haralick-analysis <csv input filePath>" <<"\n";
		
		return -1;
	}
	
	
	arma::mat data;
	mlpack::data::Load(argv[1], data, true);
	std::cout<<"ok\n";
	
	/*
	typedef mlpack::neighbor::NSModel<mlpack::neighbor::NearestNeighborSort> KNNModel;

	// We either have to load the reference data, or we have to load the model.
	KNNModel* knn;

	knn = new KNNModel();

	KNNModel::TreeTypes tree = KNNModel::KD_TREE; //any other type of trees
	mlpack::neighbor::NeighborSearchMode searchMode = mlpack::neighbor::DUAL_TREE_MODE;
	mlpack::Timers timers;
	//bool randomBasis = false;
	double tau = 1.0 , rho = 2.0, epsilon = 1.0;
	const int X = 1;
	//set param
	//std::cout<<"ok\n";
	//knn->SearchMode() = mlpack::neighbor::DUAL_TREE_MODE;
	//std::cout<<"ok1\n";
	knn->TreeType() = tree;
	//std::cout<<"ok2\n";
	//knn->RandomBasis() = randomBasis; //bool
	knn->LeafSize() = size_t(X); //int >0
	knn->Tau() = tau; //double >=0.0
	knn->Rho() = rho; //double >=1.0
    //knn->Epsilon() = epsilon; // double >= 0.0
	knn->BuildModel(std::move(data), size_t(X), searchMode, epsilon);
	
	int k = 5;
	
	// Now run the search.
	arma::Mat<size_t> neighbors;
	arma::mat distances;
	
	knn->Search(k, neighbors, distances);
	
	
	arma::Mat<size_t> neighbors;
	arma::mat distances;
	//mlpack::neighbor::NeighborSearch<mlpack::neighbor::NearestNeighborSort, mlpack::neighbor::Manhatta> NNModel(data);
	std::cout<<"ok2\n";
	NNModel.Search(5, neighbors, distances);
	
	mlpack::Log::Info << "Search complete." << std::endl;
		
	
	return 0;
	
}
*/
