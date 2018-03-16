#ifndef _AGNES_H
#define _AGNES_H

#include <vector>


class Agnes {

private:
    class Cluster {
        private:
            std::vector<std::vector<double> > *data;
            std::vector<int> datapoints;
            int n_attributes;
            Cluster *left, *right;
        public:
            Cluster(std::vector<std::vector<double> > *data, int point);
            Cluster(std::vector<std::vector<double> > *data, Cluster *l, Cluster *r);
            double Distance(Cluster other, std::vector<std::vector<double> > *distmatrix);
            std::vector<int> *GetPoints();
            void PrintCluster();

            
            static double MinkowskiDist(std::vector<double> c1, std::vector<double> c2, int n);
    };  

    std::vector<std::vector<double> > data;
    int n_clusters;
    int n_attributes;
    std::vector<Cluster*> clusters;
    void PrintRow(int n);
    

public:

    Agnes(int n);
    void Fit(double *arr, int rows, int cols);
    void GetLabels(int *out, int n);
};

#endif