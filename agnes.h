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
            double SingleLink(Cluster *other, std::vector<std::vector<double> > *distmatrix);
            double CompleteLink(Cluster *other, std::vector<std::vector<double> > *distmatrix);
            double AverageLink(Cluster *other, std::vector<std::vector<double> > *distmatrix);
            std::vector<int> *GetPoints();
            void PrintCluster();
            int GetID();

            
            static double MinkowskiDist(std::vector<double> c1, std::vector<double> c2, int n);
    };  

    std::vector<std::vector<double> > data;
    int n_clusters;
    int n_attributes;
    std::string algorithm;
    std::map<int, Cluster*> clusters;
    void PrintRow(int n);
    void FitSingleLink(std::vector<std::vector<double> > *distmatrix);
    void FitCompleteLink(std::vector<std::vector<double> > *distmatrix);
    void FitAverageLink(std::vector<std::vector<double> > *distmatrix);    

public:

    Agnes(int n, char* alg);
    void Fit(double *arr, int rows, int cols);
    void GetLabels(int *out, int n);
};

#endif