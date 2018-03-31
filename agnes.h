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
        Cluster(int point);
        Cluster(Cluster *l, Cluster *r);
        virtual ~Cluster() {;}
        virtual double Distance(Cluster *other, std::vector<std::vector<double> > *distmatrix) {return 0;}
        std::vector<int> *GetPoints();
        int GetID();
        void PrintDotGraph();

        
        static double MinkowskiDist(double *c1, double *c2, int cols, int n);
};  

class SLCluster : public Cluster {
    public:
    SLCluster(int point)  : Cluster(point) {;}
    SLCluster(Cluster *l, Cluster *r)  : Cluster(l, r) {;}
    double Distance(Cluster *other, std::vector<std::vector<double> > *distmatrix);
};


class CLCluster : public Cluster {
    public:
    CLCluster(int point) : Cluster(point) {;}
    CLCluster(Cluster *l, Cluster *r)  : Cluster(l, r) {;}
    double Distance(Cluster *other, std::vector<std::vector<double> > *distmatrix);
};


class ALCluster : public Cluster {
    public:
    ALCluster(int point)  : Cluster(point) {;}
    ALCluster(Cluster *l, Cluster *r)  : Cluster(l, r) {;}
    double Distance(Cluster *other, std::vector<std::vector<double> > *distmatrix);
};

class ClusterFactory {
    public:
    virtual ~ClusterFactory() {;}
    virtual Cluster *NewCluster(int point) = 0;
    virtual Cluster *NewCluster(Cluster *l, Cluster *r) = 0;

};

class SingleLinkFactory : public ClusterFactory {
    public:
    Cluster *NewCluster(int point) { return new SLCluster(point); }
    Cluster *NewCluster(Cluster *l, Cluster *r) { return new SLCluster(l, r); }
};

class CompleteLinkFactory : public ClusterFactory {
    public:
    Cluster *NewCluster(int point) { return new CLCluster(point); }
    Cluster *NewCluster(Cluster *l, Cluster *r) { return new CLCluster(l, r); }
};

class AverageLinkFactory : public ClusterFactory {
    public:
    Cluster *NewCluster(int point) { return new ALCluster(point); }
    Cluster *NewCluster(Cluster *l, Cluster *r) { return new ALCluster(l, r); }
};



class Agnes {

private:


    void InitDataStructures(double *arr, int rows, int cols);
    void PrecomputeDistances(double *arr, int rows, int cols);

    Cluster *GetNextNearest(Cluster *active_cluster);
    Cluster *MergeNearestClusters();
    Cluster *GetActiveCluster();

    bool ClusterIsInChain(Cluster *next_nearest);

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