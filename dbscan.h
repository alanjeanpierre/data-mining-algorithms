#ifndef _DBSCAN_H
#define _DBSCAN_H

#include <vector>
#include <map>

class DBSCAN {

private:
	bool isCoreObject(double point);
	std::vector<int> * DensityReach(int index);
	std::vector<std::vector<double> > * data;
	std::vector<std::vector<double> > * distmatrix;
	std::vector<bool> * classified;
	std::vector<double> * core;
	std::vector<double> * border;
	std::vector<double> * outlier;
	int radius;
	int minPts;
	int n_clusters;
	int n_attributes;
	int n_datapoints;

public:
	DBSCAN(int eps, int minPoints);
	void Fit(double * arr, int rows, int cols);
	void GetLabels(int * out, int n); 
};

#endif
