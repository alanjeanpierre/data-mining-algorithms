#ifndef _AGNES_H
#define _AGNES_H

#include <vector>
#include <map>
#include <iostream>
#include <sstream>

class Cluster {
    protected:
        std::vector<int> datapoints;
        double distance;
        Cluster *left, *right;
        int id;
    public:
        Cluster(int point);
        Cluster(Cluster *l, Cluster *r, double d);
        virtual ~Cluster() { delete left; delete right; }
        virtual double Distance(Cluster *other, std::vector<std::vector<double> > *distmatrix) {return 0;}
        double GetIntraClusterDistance() { return distance; }
        std::vector<int> *GetPoints();
        int GetID();
        std::string PrintDotGraph();
        Cluster *GetLeft() { return left; }
        Cluster *GetRight() { return right; }

        static double *data;
        static int n_attributes;
        static double MinkowskiDist(double *c1, double *c2, int cols, int n);
        static double MinkowskiDist(std::vector<double> *c1, std::vector<double> *c2, int n);
};  

class SLCluster : public Cluster {
    public:
    SLCluster(int point)  : Cluster(point) {;}
    SLCluster(Cluster *l, Cluster *r, double d)  : Cluster(l, r, d) {;}
    double Distance(Cluster *other, std::vector<std::vector<double> > *distmatrix);
};


class CLCluster : public Cluster {
    public:
    CLCluster(int point) : Cluster(point) {;}
    CLCluster(Cluster *l, Cluster *r, double d)  : Cluster(l, r, d) {;}
    double Distance(Cluster *other, std::vector<std::vector<double> > *distmatrix);
};


class ALCluster : public Cluster {
    public:
    ALCluster(int point)  : Cluster(point) {;}
    ALCluster(Cluster *l, Cluster *r, double d)  : Cluster(l, r, d) {;}
    double Distance(Cluster *other, std::vector<std::vector<double> > *distmatrix);
};

class WLCluster : public Cluster {
    private:
    std::vector<double> *centroid;
    public:
    WLCluster(int point); 
    WLCluster(Cluster *l, Cluster *r, double d);
    ~WLCluster() { delete centroid; }
    double Distance(Cluster *other, std::vector<std::vector<double> > *distmatrix);
};

class ClusterFactory {
    public:
    virtual ~ClusterFactory() {;}
    virtual Cluster *NewCluster(int point) = 0;
    virtual Cluster *NewCluster(Cluster *l, Cluster *r, double d) = 0;

};

template <class T>
class ClusterTemplateFactory : public ClusterFactory {
    public:
    Cluster *NewCluster(int point) { return new T(point); }
    Cluster *NewCluster(Cluster *l, Cluster *r, double d) { return new T(l, r, d); }
};


class Agnes {

private:


    void InitDataStructures(double *arr, int rows, int cols);
    void PrecomputeDistances(double *arr, int rows, int cols);
    void Clear();

    double GetNextNearest(Cluster *active_cluster, Cluster **next_nearest);
    Cluster *MergeNearestClusters(double d);
    Cluster *GetActiveCluster();

    bool ClusterIsInChain(Cluster *next_nearest);

    std::vector<std::vector<double> > *distmatrix;
    std::map<int, Cluster*> clusters;
    std::map<int, Cluster*> final_clusters;
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
    void InferLabels(int *out, int n);
    std::string PrintDotGraph(char* args=NULL);
};

#endif