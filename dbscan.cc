#include "dbscan.h"
#include "numpy_helper.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <map>
#include <algorithm>

/*
Core Cluster -- if the cluster has more than minPoints in or touching it

Border Cluster -- the cluster has less than minPoints in or touching it

Oulier Cluster -- not inside border/core cluster
*/

DBSCAN::DBSCAN(int eps, int minPoints){	
<<<<<<< HEAD
	//check eps and minPoints 
	if(eps < 1 || minPoints < 1){
		eps = 1;
		minPoints = 1;
		std::cerr << "Error: values must be positive non-zero" << std::end1;
	}

	// save the values
	this->radius = eps;
	this->minPts = minPoints;
=======
	radius = eps;
	minPts = minPoints;

	data = new std::vector<std::vector<double> >();
	
>>>>>>> 297505de8bb9469ca2c84f4b59920da72589019e
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
	
}

std::vector<int> * DBSCAN::DensityReach(int index){

}

/*
Check to see if a certain point is inside of a core cluster
*/
bool DBSCAN::isCoreObject(double point){
	return std::find(core.begin(), core.end(), point) != core.end();
}

void DBSCAN::GetLabels(int * out, int n){

} 
