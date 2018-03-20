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
	
}

std::vector<int> * DBSCAN::DensityReach(int index){

}

bool DBSCAN::isCoreObject(int index){

}

void DBSCAN::GetLabels(int * out, int n){

} 
