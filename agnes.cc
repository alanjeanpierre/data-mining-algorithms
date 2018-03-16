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

#ifdef _DEBUG
    std::cerr << "Fitting with " << rows << " x " << cols << " points " << std::endl;
#endif

    std::vector<std::vector<double> > *distmatrix = new std::vector<std::vector<double> >();
    distmatrix->resize(rows);
    clusters.resize(rows);

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
        clusters[i] = new Cluster(&data, i);
        
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

    #ifdef _DEBUG
    int iter = 1;
    #endif
    while(clusters.size() > (unsigned int)n_clusters) {
        #ifdef _DEBUG
        std::cerr << "Iteration " << iter++ << std::endl;
        #endif
        double min = 1 << 30;
        int min_i = 0;
        int min_j = 0;
        for (unsigned int i = 0; i < clusters.size(); i++) {
            //adjmatrix[i][i] = 1 << 30; // maybe use some max double?
            for (unsigned int j = 0; j < i; j++) {
                //std::cerr << "Comparing cluster " << i << " against cluster " << j << std::endl;
                double t = clusters[i]->Distance(*clusters[j], distmatrix);

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

        #ifdef _DEBUG
        std::cerr << "Merging clusters " << min_i << " and " << min_j << std::endl;
        l->PrintCluster();
        std::cerr << std::endl;
        r->PrintCluster();
        std::cerr << std::endl;
        #endif
        

        clusters.push_back(new Cluster(&data, l, r));
    }


}

void Agnes::GetLabels(int *out, int n) {

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

    for (int i = 0; i < n_clusters; i++) {
        delete clustersets[i];
    }
    delete clustersets;

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


double Agnes::Cluster::Distance(Cluster other, std::vector<std::vector<double> > *distmatrix) {
    double min = 1 << 30;
    for (unsigned int i = 0; i < datapoints.size(); i++) {
        for (unsigned int j = 0; j < other.datapoints.size(); j++) {
            double d = distmatrix->at(datapoints[i])[other.datapoints[j]];
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
