#include "dbscan.h"
#include "numpy_helper.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <map>
#include <algorithm>
 #include <cstdlib>

/*
Core Cluster -- if the cluster has more than minPoints in or touching it

Border Cluster -- the cluster has less than minPoints in or touching it

Oulier Cluster -- not inside border/core cluster
*/

DBSCAN::DBSCAN(int eps, int minPoints){	
	//check eps and minPoints 
	if(eps < 1 || minPoints < 1){
		eps = 1;
		minPoints = 1;
		std::cerr << "Error: values must be positive non-zero" << std::endl;
	}

	// save the values
	radius = eps;
	minPts = minPoints;

	data = new std::vector<std::vector<double> >();
	
}

DBSCAN::~DBSCAN() {
	delete data;
}

void DBSCAN::CopyData(double *arr, int rows, int cols) {
	for (int i = 0; i < rows; i++) {
		std::vector<double> t(cols); 
		for (int j = 0; j < cols; j++) {
			t[j] = arr[index(cols, i, j)];
		}
		data->push_back(t);
	}
}

void DBSCAN::Fit(double * arr, int rows, int cols){
	n_attributes = cols;
	n_datapoints = rows;

	// copy data to internal data structure
	CopyData(arr, rows, cols);

	// choose a random point
	double rand = 0.0;
	
	// loop through until every point is classified
	while(classified->size() != data->size()){ //not every point is classified
		if(isClassified(rand))
			continue;
		else{
			neighborhood = Neighborhood(rand);
			if(neighborhood->size() < minPts) //not core
				noise->push_back(rand);
			else{ //core point
				//start cluster around core point
			}
		}
	}
	
}

/*
Point 'p' is density reachable from point 'q' if point 'p' is within eps distance from 'q' and 
'q' has minPoints in its neighbors within distance eps
*/
std::vector<int> * DBSCAN::DensityReach(int p, int q){

}

/*
Find the neighboorhood of a point and return it in a vector; Same thing as DensityReach
*/
std::vector<double> * DBSCAN::Neighborhood(int index){
	std::vector<double> neighbors;


}

/*
Check to see if a certain point is a core point
*/
bool DBSCAN::isCoreObject(double point){
	if(std::find(core->begin(), core->end(), point) != core->end()){
		classified->push_back(point);
		return true;
	}else
		return false;
}

/*
Check to see if a certain point is inside of a border cluster
*/
bool DBSCAN::isBorderObject(double point){
	if(std::find(border->begin(), border->end(), point) != border->end()){
		classified->push_back(point);
		return true;
	}else
		return false;
	
}

/*
Check to see if a certain point is inside of a outlier cluster
*/
bool DBSCAN::isOutlierObject(double point){
	if(std::find(noise->begin(), noise->end(), point) != noise->end()){
		classified->push_back(point);
		return true;
	}else
		return false;
}

bool DBSCAN::isClassified(double point){
	return std::find(classified->begin(), classified->end(), point) != classified->end();
}

void DBSCAN::GetLabels(int * out, int n){

} 
