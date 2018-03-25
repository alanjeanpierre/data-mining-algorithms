#ifndef _DBSCAN_H
#define _DBSCAN_H

#include <vector>
#include <map>

class DBSCAN {

private:
	bool isCoreObject(double point);
	bool isBorderObject(double point);
	bool isOutlierObject(double point);
	bool isClassified(double point);
	void CopyData(double *arr, int rows, int cols);
	std::vector<double> * Neighborhood(int index);
	std::vector<int> * DensityReach(int p, int q);
	std::vector<std::vector<double> > * data;
	std::vector<std::vector<double> > * distmatrix;
	std::vector<double> * classified;
	std::vector<double> * neighborhood;
	std::vector<double> * noise;
	std::vector<double> * core;
	std::vector<double> * border;
	int radius;
	int minPts;
	int n_clusters;
	int n_attributes;
	int n_datapoints;

public:
	DBSCAN(int eps, int minPoints);
	~DBSCAN();
	void Fit(double * arr, int rows, int cols);
	void GetLabels(int * out, int n); 
};

#endif
