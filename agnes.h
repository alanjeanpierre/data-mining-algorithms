#ifndef _AGNES_H
#define _AGNES_H

#include <vector>
#include <map>
#include <iostream>

class Cluster {
    protected:
        std::vector<std::vector<double> > *data;
        std::vector<int> datapoints;
        int n_attributes;
        Cluster *left, *right;
        int id;
    public:
        Cluster(std::vector<std::vector<double> > *data, int point);
        Cluster(std::vector<std::vector<double> > *data, Cluster *l, Cluster *r);
        virtual ~Cluster() {;}
        virtual double Distance(Cluster *other, std::vector<std::vector<double> > *distmatrix) {return 0;}
        std::vector<int> *GetPoints();
        void PrintCluster();
        int GetID();
        void PrintDotGraph();

        
        static double MinkowskiDist(std::vector<double> c1, std::vector<double> c2, int n);
};  

class SLCluster : public Cluster {
    public:
    SLCluster(std::vector<std::vector<double> > *data, int point)  : Cluster(data, point) {;}
    SLCluster(std::vector<std::vector<double> > *data, Cluster *l, Cluster *r)  : Cluster(data, l, r) {;}
    double Distance(Cluster *other, std::vector<std::vector<double> > *distmatrix);
};


class CLCluster : public Cluster {
    public:
    CLCluster(std::vector<std::vector<double> > *data, int point) : Cluster(data, point) {;}
    CLCluster(std::vector<std::vector<double> > *data, Cluster *l, Cluster *r)  : Cluster(data, l, r) {;}
    double Distance(Cluster *other, std::vector<std::vector<double> > *distmatrix);
};


class ALCluster : public Cluster {
    public:
    ALCluster(std::vector<std::vector<double> > *data, int point)  : Cluster(data, point) {;}
    ALCluster(std::vector<std::vector<double> > *data, Cluster *l, Cluster *r)  : Cluster(data, l, r) {;}
    double Distance(Cluster *other, std::vector<std::vector<double> > *distmatrix);
};

class ClusterFactory {
    public:
    virtual ~ClusterFactory() {;}
    virtual Cluster *NewCluster(std::vector<std::vector<double> > *data, int point) = 0;
    virtual Cluster *NewCluster(std::vector<std::vector<double> > *data, Cluster *l, Cluster *r) = 0;

};

class SingleLinkFactory : public ClusterFactory {
    public:
    Cluster *NewCluster(std::vector<std::vector<double> > *data, int point) { return new SLCluster(data, point); }
    Cluster *NewCluster(std::vector<std::vector<double> > *data, Cluster *l, Cluster *r) { return new SLCluster(data, l, r); }
};

class CompleteLinkFactory : public ClusterFactory {
    public:
    Cluster *NewCluster(std::vector<std::vector<double> > *data, int point) { return new CLCluster(data, point); }
    Cluster *NewCluster(std::vector<std::vector<double> > *data, Cluster *l, Cluster *r) { return new CLCluster(data, l, r); }
};

class AverageLinkFactory : public ClusterFactory {
    public:
    Cluster *NewCluster(std::vector<std::vector<double> > *data, int point) { return new ALCluster(data, point); }
    Cluster *NewCluster(std::vector<std::vector<double> > *data, Cluster *l, Cluster *r) { return new ALCluster(data, l, r); }
};



class Agnes {

private:


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
    ClusterFactory *factory;

public:

    Agnes(int n, char* alg);
    ~Agnes();
    void Fit(double *arr, int rows, int cols);
    void GetLabels(int *out, int n);
    void PrintDotGraph(char* args=NULL);
};

#endif