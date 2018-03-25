#include "agnes.h"
#include "numpy_helper.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <unordered_set>
#include <map>
#include <algorithm>

static int id_counter = 0;

static double (*Linkage)(double, double, double);

double SingleLinkDistance(double single, double complete, double average) {
    return single;
}

double CompleteLinkDistance(double single, double complete, double average) {
    return complete;
}

double AverageLinkDistance(double single, double complete, double average) {
    return average;
}


Agnes::Agnes(int n, char* alg) {

    if (n < 1) {
        n = 1;
        std::cerr << "Error: values must be positive non-zero" << std::endl;
    }

    n_clusters = n;
    
    distmatrix = new std::vector<std::vector<double> >();
    data = new std::vector<std::vector<double> >();
    if (alg) {
        algorithm = std::string(alg);
        if (algorithm.compare("single") == 0)
            Linkage = SingleLinkDistance;
        else if (algorithm.compare("complete") == 0)
            Linkage = CompleteLinkDistance;
        else if(algorithm.compare("average") == 0)
            Linkage = AverageLinkDistance;
        else {
            std::cerr << "Error: invalid algorithm " << algorithm << ", defaulting to single link" << std::endl;
            Linkage = SingleLinkDistance;
        }
    } else {
        algorithm = std::string("single");
        Linkage = SingleLinkDistance;
    }
    
}

Agnes::~Agnes() {
    for(std::map<int, Cluster*>::iterator it = clusters.begin(); it != clusters.end(); it++) {
        delete it->second;
        it->second = NULL;
    }
    clusters.clear();
    delete data;
    delete distmatrix;
    data = NULL;
    distmatrix = NULL;
}

void Agnes::InitDataStructures(double *arr, int rows, int cols) {

    distmatrix->resize(rows);

    data->resize(rows);

    for(std::map<int, Cluster*>::iterator it = clusters.begin(); it != clusters.end(); it++) {
        delete it->second;
        it->second = NULL;
    }
    clusters.clear();

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
        // add row to table
        data->at(i) = t;

        // initialize single clusters
        Cluster *tmp = new Cluster(data, i);

        clusters.insert(std::pair<int, Cluster*>(tmp->GetID(), tmp));
        
        // increment data points
        d += i*n_attributes;

        // initialize ith row of adjmatrix
        distmatrix->at(i).resize(rows);
    }
}

void Agnes::PrecomputeDistances() {
    // precompute point-point distance
    for (int i = 0; i < n_datapoints; i++) {
        for (int j = 0; j < i; j++) {
            double t = Cluster::MinkowskiDist(data->at(i), data->at(j), 2);  
            distmatrix->at(i)[j] = t;
            distmatrix->at(j)[i] = t;
        }
    }
}

Agnes::Cluster *Agnes::NextNearest(Cluster *active_cluster) {
    Cluster *next_nearest = clusters.end()->second;
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
            next_nearest = it->second;
        }
    }

    return next_nearest;
}

void Agnes::Fit(double *arr, int rows, int cols) {
    n_attributes = cols;
    n_datapoints = rows;

    #ifdef _DEBUG
        std::cerr << "Fitting with " << rows << " x " << cols << " points " << std::endl;
    #endif

    InitDataStructures(arr, rows, cols);
    PrecomputeDistances();

    std::vector<Cluster*> NNChain;

    #ifdef _DEBUG
    int iter = 1;
    #endif
    while(clusters.size() > (unsigned int)n_clusters) {
        #ifdef _DEBUG
        std::cerr << "Iteration " << iter++ << std::endl;
        #endif

        if (NNChain.size() == 0)
            NNChain.push_back(clusters.begin()->second);

        Cluster *active_cluster = NNChain.back();
        Cluster *next_nearest = NextNearest(active_cluster);
        

        #ifdef _DEBUG
        std::cerr << "Found nearest to " << active_cluster->GetID() << ": " <<next_nearest->GetID() << std::endl;
        #endif

        if (std::find(NNChain.begin(), NNChain.end(), next_nearest) == NNChain.end()) {
            // stack does not contain next nearest
            NNChain.push_back(next_nearest);
            #ifdef _DEBUG
            std::cerr << "Stack doesn't contain nearest" << std::endl;
            #endif
            continue;
        }

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
        l->PrintCluster();
        std::cerr << std::endl;
        r->PrintCluster();
        std::cerr << std::endl;
        #endif
        
        // don't push merged cluster onto stack,
        // just add to cluster list
        Cluster *tmp = new Cluster(data, l, r);
        clusters.insert(std::pair<int, Cluster*>(tmp->GetID(), tmp));
    }
    id_counter = 0;
}

void Agnes::GetLabels(int *out, int n) {

    int clust_num = 0;
    std::map<int, int> *clustermap = new std::map<int, int>(); 
    for (std::map<int, Cluster*>::iterator it = clusters.begin(); it != clusters.end(); it++, clust_num++) {
        std::vector<int> *pts = it->second->GetPoints();
        for (unsigned int i = 0; i < pts->size(); i++) {
            clustermap->insert(std::pair<int, int>(pts->at(i), clust_num));
        }
    }


    for (int i = 0; i < n; i++) {
        out[i] = clustermap->find(i)->second;
    }

}

void Agnes::PrintRow(int n) {
    for (unsigned int i = 0; i < data->at(n).size(); i++)
        std::cerr << data->at(n)[i] << " ";
    std::cerr << std::endl;
}

Agnes::Cluster::Cluster(std::vector<std::vector<double> > *data, int point) {
    this->data = data;
    datapoints.push_back(point);
    id = id_counter++;
    left = NULL;
    right = NULL;
}

std::vector<int> *Agnes::Cluster::GetPoints() {
    return &datapoints;
}

Agnes::Cluster::Cluster(std::vector<std::vector<double> > *data, Cluster *l, Cluster *r) {
    this->data = data;
    if (l)
        datapoints.insert(datapoints.end(), l->datapoints.begin(), l->datapoints.end());
    if (r)
        datapoints.insert(datapoints.end(), r->datapoints.begin(), r->datapoints.end());
    
    left = l;
    right = r;
    id = id_counter++;
}

int Agnes::Cluster::GetID() {
    return id;
}


double Agnes::Cluster::Distance(Cluster *other, std::vector<std::vector<double> > *distmatrix) {
    double min = 1 << 30;
    double max = 0;
    double avg = 0;
    int n = datapoints.size() * other->datapoints.size();
    for (unsigned int i = 0; i < datapoints.size(); i++) {
        for (unsigned int j = 0; j < other->datapoints.size(); j++) {
            double d = distmatrix->at(datapoints[i])[other->datapoints[j]];
            if (d < min) 
                min = d;
            if (d > max)
                max = d;
            avg += d;
        }
    }

    return Linkage(min, max, avg/n);
}

double Agnes::Cluster::MinkowskiDist(std::vector<double> c1, std::vector<double> c2, int n) {
    double s = 0;

    for (unsigned int i = 0; i < c1.size(); i++) {
        s += std::pow(std::abs(c1[i]-c2[i]), 1.0/n);
    }
    
    return std::pow(s, 1.0/n);
}

void Agnes::Cluster::PrintCluster() {
    for (unsigned int i = 0; i < datapoints.size(); i++) {
        for (unsigned int j = 0; j < data->at(datapoints[i]).size(); j++) {
            std::cerr << data->at(datapoints[i])[j] << " ";
        }
        std::cerr << std::endl;
    }
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
    for (unsigned int i = 0; i < data->size(); i++) {
        std::cout << i << "; ";
    }
    std::cout << "}" << std::endl;

    std::cout << "}" << std::endl;
}

void Agnes::Cluster::PrintDotGraph() {
    if (left) {
        std::cout << GetID() << " -> " << left->GetID() << ";" << std::endl;
        left->PrintDotGraph();
    }
    if (right) {
        std::cout << GetID() << " -> " << right->GetID() << ";" << std::endl;
        right->PrintDotGraph();

    }
}