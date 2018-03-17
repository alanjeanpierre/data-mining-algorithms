#include "agnes.h"
#include "numpy_helper.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <unordered_set>
#include <map>
#include <algorithm>

static int id_counter;

Agnes::Agnes(int n) {

    if (n < 1) {
        n = 1;
        std::cerr << "Error: values must be positive non-zero" << std::endl;
    }

    n_clusters = n;
}

void Agnes::Fit(double *arr, int rows, int cols) {
    n_attributes = cols;

#ifdef _DEBUG
    std::cerr << "Fitting with " << rows << " x " << cols << " points " << std::endl;
#endif

    std::vector<std::vector<double> > *distmatrix = new std::vector<std::vector<double> >();
    distmatrix->resize(rows);
    //clusters.resize(rows);

    // build initial single value clusters
    double *d = arr;
    for (int i = 0; i < rows; i++) {
        std::vector<double> t;
        
        #ifdef _DEBUG
        std::cerr << "Adding row " << i << ", val: ";
        #endif
        for (int j = 0; j < n_attributes; j++) {
            // add values to row
            t.push_back(arr[index(n_attributes, i, j)]);
            #ifdef _DEBUG
            std::cerr << arr[index(n_attributes, i, j)] << " ";
            #endif
        }
        
        #ifdef _DEBUG
        std::cerr << std::endl;
        #endif 
        // add row to table
        data.push_back(t);
        //PrintRow(i);

        // initialize single clusters
        Cluster *tmp = new Cluster(&data, i);

        clusters.insert(std::pair<int, Cluster*>(tmp->GetID(), tmp));
        
        // increment data points
        d += i*n_attributes;

        // initialize ith row of adjmatrix
        distmatrix->at(i).resize(rows);
    }


    // precompute point-point distance
    for (int i = 0; i < rows; i++) {
        //adjmatrix[i][i] = 1 << 30; // maybe use some max double?
        for (int j = 0; j < i; j++) {
            //std::cerr << "Comparing cluster " << i << " against cluster " << j << std::endl;
            double t = Cluster::MinkowskiDist(data[i], data[j], 2);  
            distmatrix->at(i)[j] = t;
            distmatrix->at(j)[i] = t;
        }
    }

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
        Cluster *next_nearest;
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

        Cluster *l = NNChain.back();
        NNChain.pop_back();
        Cluster *r = NNChain.back();
        NNChain.pop_back();

        clusters.erase(l->GetID());
        clusters.erase(r->GetID());

        #ifdef _DEBUG
        std::cerr << "Merging clusters " << l->GetID() << " and " << r->GetID() << std::endl;
        l->PrintCluster();
        std::cerr << std::endl;
        r->PrintCluster();
        std::cerr << std::endl;
        #endif
        
        Cluster *tmp = new Cluster(&data, l, r);
        NNChain.push_back(tmp);
        clusters.insert(std::pair<int, Cluster*>(tmp->GetID(), tmp));
    }


}

void Agnes::GetLabels(int *out, int n) {

    std::map<int, int> *clustermap = new std::map<int, int>(); 
    int clust_num = 0;
    for (std::map<int, Cluster*>::iterator it = clusters.begin(); it != clusters.end(); it++, clust_num++) {
        std::vector<int> *pts = it->second->GetPoints();
        for (int i = 0; i < pts->size(); i++) {
            clustermap->insert(std::pair<int, int>(pts->at(i), clust_num));
        }
    }


    for (int i = 0; i < n; i++) {
        out[i] = clustermap->find(i)->second;
    }

}

void Agnes::PrintRow(int n) {
    for (unsigned int i = 0; i < data[n].size(); i++)
        std::cerr << data[n][i] << " ";
    std::cerr << std::endl;
}

Agnes::Cluster::Cluster(std::vector<std::vector<double> > *data, int point) {
    this->data = data;
    datapoints.push_back(point);
    id = id_counter++;
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
    for (unsigned int i = 0; i < datapoints.size(); i++) {
        for (unsigned int j = 0; j < other->datapoints.size(); j++) {
            double d = distmatrix->at(datapoints[i])[other->datapoints[j]];
            if (d < min) 
                min = d;
        }
    }

    return min;

    
}


// should precompte this for the whole table
// instead of running each time
// it's literally O(k*n^5) right now with a huge constant too because of fp and division
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
