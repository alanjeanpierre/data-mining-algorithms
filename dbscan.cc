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
	
}

void DBSCAN::Fit(double * arr, int rows, int cols){
	n_attributes = cols;
	n_datapoints = rows;
	
}

std::vector<int> * DBSCAN::DensityReach(int index){

}

bool DBSCAN::isCoreObject(int index){

}

void DBSCAN::GetLabels(int * out, int n){

} 
