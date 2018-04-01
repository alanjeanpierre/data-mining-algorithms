#include "agnes.h"
#include "numpy_helper.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <unordered_set>
#include <map>
#include <algorithm>
#include <cassert>

static int id_counter = 0;
double *Cluster::data = NULL;
int Cluster::n_attributes = 0;

Agnes::Agnes(int n, char* alg) {

    #ifdef _DEBUG
    std::cerr << "Creating Agnes object targetting " << n << " clusters using " << alg << " linkage" <<std::endl;
    #endif

    if (n < 1) {
        n = 1;
        std::cerr << "Error: values must be positive non-zero" << std::endl;
    }

    n_clusters = n;
    distmatrix = NULL;
    if (alg) {
        algorithm = std::string(alg);
        if (algorithm.compare("single") == 0)
            factory = new SingleLinkFactory();
        else if (algorithm.compare("complete") == 0)
            factory = new CompleteLinkFactory();
        else if(algorithm.compare("average") == 0)
            factory = new AverageLinkFactory();
        else if(algorithm.compare("wards") == 0) {
            factory = new WardsLinkFactory();
        }
        else {
            std::cerr << "Error: invalid algorithm " << alg << ", defaulting to single link" << std::endl;
            factory = new SingleLinkFactory();
        }
    } else {
        algorithm = std::string("single");
        factory = new SingleLinkFactory();
    }
}

void Agnes::Clear() {
    for(std::map<int, Cluster*>::iterator it = clusters.begin(); it != clusters.end(); it++) {
        delete it->second;
        it->second = NULL;
    }
    clusters.clear();
    final_clusters.clear();
    delete distmatrix;
    distmatrix = NULL;
    id_counter = 0;
}

Agnes::~Agnes() {
    Clear();
}

void Agnes::InitDataStructures(double *arr, int rows, int cols) {

    Clear();
    distmatrix = new std::vector<std::vector<double> >();
    distmatrix->resize(rows);
    Cluster::data = arr;
    Cluster::n_attributes = cols;

    // build initial single value clusters
    double *d = arr;
    for (int i = 0; i < rows; i++) {
        std::vector<double> t(cols, 0);
        
        #ifdef _DEBUG
        std::cerr << "Adding row " << i << ", val: ";
        #endif
        for (int j = 0; j < n_attributes; j++) {
            // add values to row
            t[j] = arr[index(n_attributes, i, j)];
            #ifdef _DEBUG
            std::cerr << arr[index(n_attributes, i, j)] << " ";
            #endif
        }
        
        #ifdef _DEBUG
        std::cerr << std::endl;
        #endif 

        // initialize single clusters
        Cluster *tmp = factory->NewCluster(i);
        

        clusters.insert(std::pair<int, Cluster*>(tmp->GetID(), tmp));
        
        // increment data points
        d += i*n_attributes;

        // initialize ith row of adjmatrix
        distmatrix->at(i).resize(rows);
    }
}

// theta(dn^2), n=n_cluster, d=n_attributes
void Agnes::PrecomputeDistances(double *arr, int rows, int cols) {
    // precompute point-point distance
    double *d1, *d2;
    for (int i = 0; i < n_datapoints; i++) {
        d1 = arr + i*cols;
        for (int j = 0; j < i; j++) {
            d2 = arr + j*cols;
            double t = Cluster::MinkowskiDist(d1, d2, cols, 2);  
            distmatrix->at(i)[j] = t;
            distmatrix->at(j)[i] = t;
        }
    }
}

// O(c)
double Agnes::GetNextNearest(Cluster *active_cluster, Cluster **next_nearest) {
    *next_nearest = clusters.end()->second;
    double min = 1 << 30;
    for(std::map<int, Cluster*>::iterator it = clusters.begin(); it != clusters.end(); it++) {
        if (active_cluster->GetID() == it->second->GetID())
            continue;
        double t = active_cluster->Distance(it->second, distmatrix);
        #ifdef _DEBUG
            std::cerr << "Distance: " << active_cluster->GetID() << ":" << it->second->GetID() << " = " << t << std::endl;
        #endif
        if (t < min) {
            min = t;
            *next_nearest = it->second;
        }
    }

    return min;
}

Cluster *Agnes::GetActiveCluster() {

    if (NNChain.size() == 0) {
        auto it = clusters.begin();
        NNChain.push_back(it->second);
    }

    Cluster *active_cluster = NNChain.back();
    return active_cluster;
}

bool Agnes::ClusterIsInChain(Cluster *next_nearest) {
    // O(c)

    return std::find(NNChain.begin(), NNChain.end(), next_nearest) == NNChain.end();

}

Cluster *Agnes::MergeNearestClusters(double d) {
    // Remove top 2 nearest clusters from stack
    Cluster *l = NNChain.back();
    NNChain.pop_back();
    Cluster *r = NNChain.back();
    NNChain.pop_back();

    // remove clusters from cluster list
    clusters.erase(l->GetID());
    clusters.erase(r->GetID());

    #ifdef _DEBUG
    std::cerr << "Merging clusters " << l->GetID() << " and " << r->GetID() << std::endl;
    #endif
    
    // don't push merged cluster onto stack,
    // just add to cluster list
    Cluster *tmp = factory->NewCluster(l, r, d);
    return tmp;
}

void Agnes::Fit(double *arr, int rows, int cols) {
    /*
     * Let:
     *      n = num points
     *      c = current num clusters
     *      d = dimensionality of the data
     * 
     * Fit = O(dn^2)
     */
    n_attributes = cols;
    n_datapoints = rows;

    #ifdef _DEBUG
    std::cerr << "Fitting with " << rows << " x " << cols << " points " << std::endl;
    int iter = 1;
    #endif

    InitDataStructures(arr, rows, cols); // O(n)
    PrecomputeDistances(arr, rows, cols);               // O(dn^2)

    // loop runs O(n) times with 2 O(c) computations per run
    // since c = O(n)
    // loop = O(n^2)
    while(clusters.size() > 1) {
        #ifdef _DEBUG
        std::cerr << "Iteration " << iter++ << std::endl;
        #endif
        
        Cluster *active_cluster = GetActiveCluster(); // O(1)
        Cluster *next_nearest;
        double d = GetNextNearest(active_cluster, &next_nearest); // O(c) 
        

        #ifdef _DEBUG
        std::cerr << "Found nearest to " << active_cluster->GetID() << ": " << next_nearest->GetID() << std::endl;
        #endif

        if (ClusterIsInChain(next_nearest)) {
            #ifdef _DEBUG
            std::cerr << "Stack doesn't contain nearest" << std::endl;
            #endif
    
            NNChain.push_back(next_nearest);
            continue;
        }

        Cluster *merged = MergeNearestClusters(d);
        clusters.insert(std::pair<int, Cluster*>(merged->GetID(), merged));
    }

    final_clusters.insert(std::pair<int, Cluster*>(clusters.begin()->first, clusters.begin()->second));
    while (final_clusters.size() < (unsigned int)n_clusters) {
        // find cluster with max distance closed between children
        double max = 0;
        Cluster *m = final_clusters.begin()->second;
        for (std::map<int, Cluster*>::iterator it = final_clusters.begin(); it != final_clusters.end(); it++) {
            double t = it->second->GetIntraClusterDistance();
            if (t > max) {
                max = t;
                m = it->second;
            }
        }

        // replace that cluster with its children
        final_clusters.erase(m->GetID());
        final_clusters.insert(std::pair<int, Cluster*>(m->GetLeft()->GetID(), m->GetLeft()));
        final_clusters.insert(std::pair<int, Cluster*>(m->GetRight()->GetID(), m->GetRight()));
    }
}

void Agnes::GetLabels(int *out, int n) {

    int clust_num = 0;
    std::map<int, int> *clustermap = new std::map<int, int>(); 
    for (std::map<int, Cluster*>::iterator it = final_clusters.begin(); it != final_clusters.end(); it++, clust_num++) {
        std::vector<int> *pts = it->second->GetPoints();
        for (unsigned int i = 0; i < pts->size(); i++) {
            clustermap->insert(std::pair<int, int>(pts->at(i), clust_num));
        }
    }


    for (int i = 0; i < n; i++) {
        out[i] = clustermap->find(i)->second;
    }

}

Cluster::Cluster(int point) {
    datapoints.push_back(point);
    id = id_counter++;
    distance = 0;
    left = NULL;
    right = NULL;
}

std::vector<int> *Cluster::GetPoints() {
    return &datapoints;
}

Cluster::Cluster(Cluster *l, Cluster *r, double d) {
    if (l)
        datapoints.insert(datapoints.end(), l->datapoints.begin(), l->datapoints.end());
    if (r)
        datapoints.insert(datapoints.end(), r->datapoints.begin(), r->datapoints.end());
    
    left = l;
    right = r;
    distance = d;
    id = id_counter++;
}

int Cluster::GetID() {
    return id;
}

double Cluster::MinkowskiDist(double *c1, double *c2, int cols, int n) {
    double s = 0;

    for (int i = 0; i < cols; i++) {
        s += std::pow(std::abs(c1[i]-c2[i]), n);
    }
    
    return std::pow(s, 1.0/n);
}

double Cluster::MinkowskiDist(std::vector<double> *c1, std::vector<double> *c2, int n) {
    double s = 0;

    for (unsigned int i = 0; i < c1->size(); i++) {
        s += std::pow(std::abs(c1->at(i)-c2->at(i)), n);
    }
    
    return std::pow(s, 1.0/n);
}

void Agnes::PrintDotGraph(char* args) {
    std::cout << "digraph G {" << std::endl;

    if (args)
        std::cout << args << std::endl;

    for(std::map<int, Cluster*>::iterator it = clusters.begin(); it != clusters.end(); it++) {
        std::cout << "root -> " << it->first << ";" << std::endl;
        it->second->PrintDotGraph(); 
    }

    std::cout << "{rank = max; ";
    for (int i = 0; i < n_datapoints; i++) {
        std::cout << i << "; ";
    }
    std::cout << "}" << std::endl;

    std::cout << "}" << std::endl;
}

void Cluster::PrintDotGraph() {
    if (left) {
        std::cout << GetID() << " -> " << left->GetID() << ";" << std::endl;
        left->PrintDotGraph();
    }
    if (right) {
        std::cout << GetID() << " -> " << right->GetID() << ";" << std::endl;
        right->PrintDotGraph();

    }
}

double SLCluster::Distance(Cluster *other, std::vector<std::vector<double> > *distmatrix) {
    double min = 1 << 30;
    std::vector<int> *o = other->GetPoints();
    for (unsigned int i = 0; i < datapoints.size(); i++) {
        for (unsigned int j = 0; j < o->size(); j++) {
            double d = distmatrix->at(datapoints[i])[o->at(j)];
            if (d < min) 
                min = d;
        }
    }

    return min;
}

double CLCluster::Distance(Cluster *other, std::vector<std::vector<double> > *distmatrix) {
    double max = 0;
    std::vector<int> *o = other->GetPoints();
    for (unsigned int i = 0; i < datapoints.size(); i++) {
        for (unsigned int j = 0; j < o->size(); j++) {
            double d = distmatrix->at(datapoints[i])[o->at(j)];
            if (d > max) 
                max = d;
        }
    }

    return max;
}

double ALCluster::Distance(Cluster *other, std::vector<std::vector<double> > *distmatrix) {
    double avg = 0;
    std::vector<int> *o = other->GetPoints();
    int n = datapoints.size() * o->size();
    for (unsigned int i = 0; i < datapoints.size(); i++) {
        for (unsigned int j = 0; j < o->size(); j++) {
            double d = distmatrix->at(datapoints[i])[o->at(j)];
            avg += d;
        }
    }

    return avg/n;
}

WLCluster::WLCluster(int point)  : Cluster(point) {
    centroid = new std::vector<double>();
    for (int i = 0; i < n_attributes; i++) {
        centroid->push_back(data[index(n_attributes, point, i)]);
    }
}

WLCluster::WLCluster(Cluster *l, Cluster *r, double d)  : Cluster(l, r, d) {
    WLCluster *L = static_cast<WLCluster*>(l);
    WLCluster *R = static_cast<WLCluster*>(r);
    centroid = new std::vector<double>();
    for (int i = 0; i < n_attributes; i++) {
        double l_i = L->centroid->at(i) * L->datapoints.size();
        double r_i = R->centroid->at(i) * R->datapoints.size();
        centroid->push_back((l_i + r_i)/(L->datapoints.size() + R->datapoints.size()));
    }
}

double WLCluster::Distance(Cluster *other, std::vector<std::vector<double> > *distmatrix) {
    WLCluster *o = static_cast<WLCluster*>(other);

    double centroid_dist = MinkowskiDist(centroid, o->centroid, 2);
    centroid_dist *= centroid_dist;

    return centroid_dist/ ( datapoints.size() + o->datapoints.size());

}