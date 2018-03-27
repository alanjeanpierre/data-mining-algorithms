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

double MinkowskiDist(std::vector<double> c1, std::vector<double> c2, int n);

DBSCAN::DBSCAN(double eps, int minPoints){	
	//check eps and minPoints 

	// save the values
	this->eps = eps;
	minPts = minPoints;

	data = new std::vector<std::vector<double> >();
	
}

DBSCAN::~DBSCAN() {
	delete data;
	data = NULL;
	while(points->size() > 0) {
		Point *p = points->back();
		points->pop_back();
		delete p;
	}
}

void DBSCAN::CopyData(double *arr, int rows, int cols) {
	for (int i = 0; i < rows; i++) {
		#ifdef _DEBUG
		std::cerr << "Adding row " << i << ", val: ";
		#endif
		std::vector<double> t(cols); 
		for (int j = 0; j < cols; j++) {
			#ifdef _DEBUG
			std::cerr << arr[index(n_attributes, i, j)] << " ";
			#endif
			t[j] = arr[index(cols, i, j)];
		}
		data->push_back(t);
		#ifdef _DEBUG
		std::cerr << std::endl;
		#endif
	}
}

double MinkowskiDist(std::vector<double> c1, std::vector<double> c2, int n) {
    double s = 0;

    for (unsigned int i = 0; i < c1.size(); i++) {
        s += std::pow(std::abs(c1[i]-c2[i]), 1.0/n);
    }
    
    return std::pow(s, 1.0/n);
}

void DBSCAN::InitDataStructures() {

	// initialize the Points
	points = new std::vector<Point*>();
	points->resize(n_datapoints);
	for (int i = 0; i < n_datapoints; i++) {
		points->at(i) = new Point(i);
	}

	// calculate the distances between points
	#ifdef _DEBUG
	std::cerr << "Calculating distance matrix " << std::endl;
	#endif
	std::vector<std::vector<double> > distmatrix;
	distmatrix.resize(n_datapoints);
	for (int i = 0; i < n_datapoints; i++) {
		distmatrix[i].resize(n_datapoints);
        for (int j = 0; j < i; j++) {
            double t = MinkowskiDist(data->at(i), data->at(j), 2);  
            distmatrix[i][j] = t;
            distmatrix[j][i] = t;
        }
    }

	// if dist < eps, add that point as a neighbor
	for (int i = 0; i < n_datapoints; i++) {
		#ifdef _DEBUG
		std::cerr << "Adding as neighbors to point #" << i << ": ";
		#endif
		for (int j = 0; j < n_datapoints; j++) {
			if (j == i) continue; // don't include self in neighbors
			if (distmatrix[i][j] < eps) {
				#ifdef _DEBUG
				std::cerr << j << " ";
				#endif
				points->at(i)->AddNeighbor(points->at(j));
			}
		}
		#ifdef _DEBUG
		std::cerr << std::endl;
		#endif
	}
}

void DBSCAN::Fit(double *arr, int rows, int cols){
	n_attributes = cols;
	n_datapoints = rows;

	#ifdef _DEBUG
	std::cerr << "Fitting with " << n_datapoints << " x " << n_attributes << " points " << std::endl;
	#endif

	// copy data to internal data structure
	CopyData(arr, rows, cols);
	InitDataStructures();

	int Clid = NextID();
	for (int i = 0; i < n_datapoints; i++) {
		Point* p = points->at(i);
		if (p->IsUnClassified()) {
			if (ExpandCluster(p, Clid)) {
				Clid = NextID();
			}
		}
	}
}

bool DBSCAN::ExpandCluster(Point* point, int ClId) {

	#ifdef _DEBUG
	std::cerr << "Examining point #" << point->GetID() << ", class=" <<	point->GetClassification() << 
	" numPts=" << point->RegionQuery()->size() <<  std::endl;
	#endif
	if (!point->IsCorePoint(minPts)) {
		#ifdef _DEBUG
		std::cerr << "Not a core point, setting class to NOISE" << std::endl;
		#endif
		point->SetClassification(NOISE);
		return false;
	}

	std::vector<Point*> *seeds = point->RegionQuery();
	point->SetClassification(ClId);
	point->SetChildrenIDs(ClId);
	while (seeds->size() > 0) {
		Point* currentP = seeds->front();

		#ifdef _DEBUG	
		std::cerr << "Examining child point #" << currentP->GetID() << ", class=" <<	currentP->GetClassification() << 
		" numPts=" << currentP->RegionQuery()->size() <<  std::endl;
		#endif
		if (currentP->IsCorePoint(minPts)) {
			
			std::vector<Point*> *result = currentP->RegionQuery();
			for (int i = 0; i < result->size(); i++) {

				Point *resultP = result->at(i);

				if (resultP->IsUnClassified() || resultP->IsNoise()) {
					if (resultP->IsUnClassified()) {
						seeds->push_back(resultP);
					}
					resultP->SetClassification(ClId);
				}

			}
		}

		seeds->erase(seeds->begin());
	}

	return true;
}

int DBSCAN::NextID() { 
	static int id = 0;
	int r = id;
	id += 1;
	return r;
}

void DBSCAN::GetLabels(int *out, int n) {
	for (int i = 0; i < n; i++) {
		out[i] = points->at(i)->GetClassification();
	}
}

DBSCAN::Point::Point(int id) {
	this->id = id;
	classification = UNCLASSIFIED; 
	neighbors = new std::vector<Point*>();
}

DBSCAN::Point::~Point() {
	delete neighbors;
	neighbors = NULL;
}

void DBSCAN::Point::AddNeighbor(Point *neighbor) {
	neighbors->push_back(neighbor);
}

void DBSCAN::Point::SetChildrenIDs(int id) {
	for (int i = 0; i < neighbors->size(); i++) {
		neighbors->at(i)->SetClassification(id);
	}
}