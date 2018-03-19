#ifndef _AGNES_H
#define _AGNES_H

#include <vector>
#include <map>




class Agnes {

private:
    class Cluster {
        private:
            std::vector<std::vector<double> > *data;
            std::vector<int> datapoints;
            int n_attributes;
            Cluster *left, *right;
            int id;
        public:
            Cluster(std::vector<std::vector<double> > *data, int point);
            Cluster(std::vector<std::vector<double> > *data, Cluster *l, Cluster *r);
            double Distance(Cluster *other, std::vector<std::vector<double> > *distmatrix);
            std::vector<int> *GetPoints();
            void PrintCluster();
            int GetID();
            void PrintDotGraph();

            
            static double MinkowskiDist(std::vector<double> c1, std::vector<double> c2, int n);
    };  

    std::vector<std::vector<double> > data;
    std::vector<std::vector<double> > *distmatrix;
    int n_clusters;
    int n_attributes;
    int n_datapoints;
    std::string algorithm;
    std::map<int, Cluster*> clusters;
    void PrintRow(int n);
    void InitDataStructures(double *arr, int rows, int cols);
    void PrecomputeDistances();
    Cluster *NextNearest(Cluster *active_cluster);

public:

    Agnes(int n, char* alg);
    void Fit(double *arr, int rows, int cols);
    void GetLabels(int *out, int n);
    void PrintDotGraph(char* args=NULL);
};

#endif