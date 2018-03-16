#include "kmeans.h"
#include <iostream>
#include <ctime>
#include <cstdlib>
#include "numpy_helper.h"
#include <cmath>

// helper function for comparing doubles
bool withinTolerance(double a, double b, double t = 0.01);


KMeans::KMeans(int n_clusters, int max_iter, int random_state) {

    if (n_clusters < 1 || max_iter < 1) {
        n_clusters = 1;
        max_iter = 1;
        std::cerr << "Error: values must be positive non-zero" << std::endl;
    }

    this->n_clusters = n_clusters;
    this->max_iter = max_iter;
    
    if (random_state == -1)
        this->rand_seed = time(NULL);
    else
        this->rand_seed = random_state;
}

void KMeans::Fit(double *invec, int n, int m) {
    
    std::srand(rand_seed);
    n_attributes = m;
    
    // allocate clusters
    for (int i = 0; i < n_clusters; i++)
        clusters.push_back(Cluster(invec, n, n_attributes));
    
    // initial random assignment of clusters
    for (int i = 0; i < n; i++) {
        int centroid = std::rand()%n_clusters;
        clusters[centroid].AddPoint(i);
    }
    
    // run until no change in clusters
    bool diff = true;
    
    for (int runs = 0; diff && runs < max_iter; runs++) {
        
        diff = false;
        // calculate centroids
        for (int i = 0; i < n_clusters; i++) {
            diff |= clusters[i].CalcCentroid();
        }
        
        //if (!diff)
        //    break;
        
        for (int i = 0; i < n_clusters; i++)
            clusters[i].ResetPoints();
        
        double *d;
        // reassign points
        for (int i = 0; i < n; i++) {
            d = invec + i * n_attributes;
           int minindex = NearestCluster(d, n_attributes);
            clusters[minindex].AddPoint(i);   
        }
    }   
}

int KMeans::NearestCluster(double *row, int n) {

    if (n != n_attributes) {
        std::cerr << "Error: num input attributes (" << n 
                  << ") doesn't match num cluster attributes (" 
                  << n_attributes << ")" << std::endl;
        return 0;
    }

     // nearest centroid
    double mindist = 1 << 30; // hsould set to some max double?
    int minindex = 0;
    for (int c = 0; c < n_clusters; c++) {
        double dist = clusters[c].MinkowskiDist(row, n);
        if (dist < mindist) {
            mindist = dist;
            minindex = c;
        }
    }
    
    return minindex;
}

void KMeans::GetClusters(double *arr, int rows, int cols) {

    if (rows != n_clusters || cols != n_attributes) {
        std::cerr << "Error: input array dimensions (" <<
                  rows << ", " << cols << ") don't match cluster dimensions ("
                  << n_clusters << ", " << n_attributes << ")!" << std::endl;
        return;
    }

    for (int i = 0; i < n_clusters; i++) {
        std::vector<double> centroid = clusters[i].GetCentroid();
        for (int j = 0; j < n_attributes; j++) {
            arr[index(cols, i, j)] = centroid[j];
        }
    }
}

int KMeans::Predict(double *row, int n) {
    return NearestCluster(row, n);
}


KMeans::Cluster::Cluster(double *data, int rows, int columns) {
    this->data = data;
    this->rows = rows;
    this->columns = columns;
    this->n_attributes = columns;
    this->n_points = 0;
    
    for (int i = 0; i < columns; i++) {
        centroid.push_back(0);
    }
    
}

void KMeans::Cluster::ResetCentroid() {

    centroid.clear();
    for (int i = 0; i < n_attributes; i++ )
        centroid.push_back(0);
}

void KMeans::Cluster::AddPoint(int row) {
    
    points.push_back(row);
    n_points += 1;
}

bool KMeans::Cluster::CalcCentroid() {
    
    
    double *old = new double[n_attributes];
    for (int i = 0; i < n_attributes; i++)
        old[i] = centroid[i];
    
    ResetCentroid();
    
    // pointing d to the start of the nth row
    double *d;
    
    // sum points
    for (int i = 0; i < n_points; i++) {
        d = data + points[i]*columns;     
        for (int j = 0; j < columns; j++) {
            centroid[j] += d[j];
        }
    }
    
    bool diff = false;
    // average
    for (int i = 0; i < columns; i++) {
        centroid[i] /= n_points;
        diff |= !withinTolerance(centroid[i], old[i], 0.1);
    }    
    
    return diff;
}

double KMeans::Cluster::MinkowskiDist(double *d, int n) {
    double s = 0;
    
    if (n != n_attributes)
        return -1;
    
    for (int i = 0; i < n_attributes; i++) {
        s += std::pow(std::abs(centroid[i]-d[i]), 1.0/n);
    }
    
    return std::sqrt(s);
}

void KMeans::Cluster::PrintCentroid() {
    std::cout << "[ " ;
    for (int i = 0; i < n_attributes; i++) 
        std::cout << centroid[i] << ", ";
    std::cout << "]" << std::endl;
}

void KMeans::Cluster::ResetPoints() {
    points.clear();
    n_points = 0;
}

int KMeans::Cluster::GetNumAttrs() {
    return n_attributes;
}

std::vector<double> KMeans::Cluster::GetCentroid() {
    return centroid;
}

// helper function for comparing doubles
bool withinTolerance(double a, double b, double t) {
    return std::abs(a - b) <= t;
}

