#include "agnes.h"
#include "numpy_helper.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <unordered_set>

Agnes::Agnes(int n) {

    if (n < 1) {
        n = 1;
        std::cerr << "Error: values must be positive non-zero" << std::endl;
    }

    n_clusters = n;
}

void Agnes::Fit(double *arr, int rows, int cols) {
    n_attributes = cols;

    //std::cerr << "Fitting with " << rows << " x " << cols << " points " << std::endl;

    //std::vector<std::vector<double> > adjmatrix;
    //adjmatrix.resize(rows);
    clusters.resize(rows);
    // build initial single value clusters
    double *d = arr;
    for (int i = 0; i < rows; i++) {
        std::vector<double> t;
        //std::cerr << "Adding row " << i << ", val: ";
        for (int j = 0; j < n_attributes; j++) {
            // add values to row
            t.push_back(arr[index(n_attributes, i, j)]);
            //std::cerr << arr[index(n_attributes, i, j)] << " ";
        }
        //std::cerr << std::endl;
        // add row to table
        data.push_back(t);
        //PrintRow(i);

        // initialize single clusters
        clusters[i] = new Cluster(&data, i);
        
        // increment data points
        d += i*n_attributes;

        // initialize ith row of adjmatrix
        //adjmatrix[i].resize(rows);
    }

    int iter = 1;
    while(clusters.size() > n_clusters) {
        double min = 1 << 30;
        int min_i = 0;
        int min_j = 0;
        for (int i = 0; i < clusters.size(); i++) {
            //adjmatrix[i][i] = 1 << 30; // maybe use some max double?
            for (int j = 0; j < i; j++) {
                //std::cerr << "Comparing cluster " << i << " against cluster " << j << std::endl;
                double t = clusters[i]->Distance(*clusters[j]);  

                //std::cerr << "Dist = " << t << std::endl << std::endl;
                if (t < min) {
                    min = t;
                    min_i = i;
                    min_j = j;
                }
            }
        }

        Cluster *l = clusters[min_i];
        Cluster *r = clusters[min_j];
        clusters[min_i] = clusters.back();
        clusters.pop_back();
        clusters[min_j] = clusters.back();
        clusters.pop_back();

        //std::cerr << "Printing merged clusters" << std::endl;
        //l->PrintCluster();
        //r->PrintCluster();

        clusters.push_back(new Cluster(&data, l, r));
/*
        std::cerr << "\nIteration " << iter++ << std::endl;
        for (unsigned int i = 0; i < clusters.size(); i++) {
            std::vector<int> *p = clusters[i]->GetPoints();
            std::cerr << "Cluster " << i << " contains x points: " << p->size() << std::endl;
            for (unsigned int j = 0; j < p->size(); j++) 
                PrintRow(p->at(j));
            std::cerr << std::endl;
        }
        std::cerr << std::endl;
*/
    }


}

void Agnes::GetLabels(double *out, int n) {

    std::unordered_set<int> **clustersets = new std::unordered_set<int>*[n_clusters];
    if (clustersets == nullptr)
        std::cerr << "Uh oh, nullptr" << std::endl;
    for (int i = 0; i < n_clusters; i++) {
        clustersets[i] = new std::unordered_set<int>(clusters[i]->GetPoints()->begin(), clusters[i]->GetPoints()->end());
        if (clustersets[i] == nullptr)
            std::cerr << "Uh oh, another nullptr" << std::endl;
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n_clusters; j++) {
            if (clustersets[j]->count(i) > 0) {
                out[i] = j;
                break;
            }
        }
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
}


double Agnes::Cluster::Distance(Cluster other) {
    double min = 1 << 30;
    for (unsigned int i = 0; i < datapoints.size(); i++) {
        for (unsigned int j = 0; j < other.datapoints.size(); j++) {
            double d = MinkowskiDist(data->at(datapoints[i]), data->at(other.datapoints[j]), 2);
            if (d < min) 
                min = d;
        }
    }

    return min;

    
}


double Agnes::Cluster::MinkowskiDist(std::vector<double> c1, std::vector<double> c2, int n) {
    double s = 0;

    for (unsigned int i = 0; i < c1.size(); i++) {
        s += std::pow(std::abs(c1[i]-c2[i]), 1.0/n);
    }
    
    return std::pow(s, 1.0/n);
}

void Agnes::Cluster::PrintCluster() {
    for (int i = 0; i < datapoints.size(); i++) {
        for (int j = 0; j < data->at(datapoints[i]).size(); j++) {
            std::cerr << data->at(datapoints[i])[j] << " ";
        }
        std::cerr << std::endl;
    }
}