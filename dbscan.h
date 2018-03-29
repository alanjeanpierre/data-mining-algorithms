#ifndef _DBSCAN_H
#define _DBSCAN_H

#include <vector>
#include <map>

#define NOISE -1
#define UNCLASSIFIED -2

class DBSCAN {

private:
	class Point {
	private:
		int classification;
		int id;
		std::vector<Point*> *neighbors;

	public:
		Point(int id);
		~Point();
		int GetID() { return id; }
		int GetClassification() { return classification; }
		void SetClassification(int c) { classification = c; }
		bool IsUnClassified() { return classification == UNCLASSIFIED; }
		bool IsNoise() { return classification == NOISE; }
		bool IsCorePoint(int minPts ) { return neighbors->size() + 1 >= minPts; }
		std::vector<Point*> *RegionQuery() { return neighbors; }
		void SetChildrenIDs(int id);
		void AddNeighbor(Point *neighbor);
	};


	void CopyData(double *arr, int rows, int cols);
	void InitDataStructures();
	bool ExpandCluster(Point* point, int ClId);
	int NextID();

	std::vector<Point*> *points;
	double eps;
	int minPts;
	int n_clusters;
	int n_attributes;
	int n_datapoints;
	std::vector<std::vector<double> > *data;

public:
	DBSCAN(double eps, int minPoints);
	~DBSCAN();
	void Fit(double *arr, int rows, int cols);
	void GetLabels(int * out, int n); 
};

#endif
