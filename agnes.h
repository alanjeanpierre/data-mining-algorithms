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

    void PrintRow(int n);
    void InitDataStructures(double *arr, int rows, int cols);
    void PrecomputeDistances();

    Cluster *GetNextNearest(Cluster *active_cluster);
    Cluster *MergeNearestClusters();
    Cluster *GetActiveCluster();

    bool ClusterIsInChain(Cluster *next_nearest);

    std::vector<std::vector<double> > *data;
    std::vector<std::vector<double> > *distmatrix;
    std::map<int, Cluster*> clusters;
    std::vector<Cluster*> NNChain;

    int n_clusters;
    int n_attributes;
    int n_datapoints;

    std::string algorithm;

public:

    Agnes(int n, char* alg);
    ~Agnes();
    void Fit(double *arr, int rows, int cols);
    void GetLabels(int *out, int n);
    void PrintDotGraph(char* args=NULL);
};

#endif