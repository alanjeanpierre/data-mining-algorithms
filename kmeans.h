#ifndef _K_MEANS
#define _K_MEANS

#include <vector>


class KMeans {
    private:

    class Cluster {
    private:
        int n_attributes;
        int n_points;
        std::vector<double> centroid;
        std::vector<int> points;
        double *data;
        int rows;
        int columns;
        
        void ResetCentroid();
        
    public:
        Cluster(double *data, int rows, int columns);
        void AddPoint(int row);
        bool CalcCentroid();
        double MinkowskiDist(double *d, int n);
        void PrintCentroid();
        void ResetPoints();
        int GetNumAttrs();
        std::vector<double> GetCentroid();
        
        
    };

        int n_clusters;
        int n_attributes;
        int max_iter;
        int rand_seed;
        std::vector<Cluster> clusters;
        
        int NearestCluster(double *row, int n);
        

    public:
        KMeans(int n_clusters=8, int max_iter=300, int random_state=-1);
        void fit(double *invec, int n, int m);
        int predict(double *row, int n);
        void GetClusters(double *arr, int rows, int cols);

};


#endif
