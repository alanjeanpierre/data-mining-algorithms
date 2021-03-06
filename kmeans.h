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
            std::vector<std::vector<double> > *data;
            int rows;
            int columns;
            
            void ResetCentroid();
            
        public:
            Cluster(std::vector<std::vector<double> > *data, int rows, int columns);
            void AddPoint(int row);
            bool CalcCentroid();
            double MinkowskiDist(std::vector<double> *datapoint, int n);
            void PrintCentroid();
            void ResetPoints();
            int GetNumAttrs();
            std::vector<double> GetCentroid();
            std::vector<int>* GetPoints();
             
            
        };

        int n_clusters;
        int n_attributes;
        int max_iter;
        int rand_seed;
        std::vector<std::vector<double> > *data;
        std::vector<Cluster> clusters;
        
        int NearestCluster(std::vector<double> *datapoint);
        void CopyData(double *invec, int n, int m);
        void PrintClusters();
        

    public:
        KMeans(int n_clusters=8, int max_iter=300, int random_state=-1);
        ~KMeans();
        void Fit(double *invec, int n, int m);
        int Predict(double *row, int n);
        void GetClusters(double *arr, int rows, int cols);
        void GetLabels(int *out, int n);

};


#endif
