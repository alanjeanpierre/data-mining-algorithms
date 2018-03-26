#ifndef _DBSCAN_H
#define _DBSCAN_H

#include <vector>
#include <map>

class DBSCAN {

private:
	bool isCoreObject(double point); //may not be needed
	bool isBorderObject(double point); //may not be needed
	bool isOutlierObject(double point);
	bool isClassified(double point);
	void CopyData(double *arr, int rows, int cols);
	double Distance(int p, int q);
	std::vector<double> * Neighborhood(double index);
	std::vector<std::vector<double> > * data;
	std::vector<std::vector<double> > * distmatrix;
	std::vector<double> * classified;
	std::vector<double> * neighborhood;
	std::vector<double> * noise; //possible change to multimap
	std::map<double, std::vector<double> > * core; //possible change to multimap
	std::map<double, std::vector<double> > * border; //possible change to multimap
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
