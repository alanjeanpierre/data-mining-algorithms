#include "kmeans.h"
#include <iostream>
#include <ctime>
#include <cstdlib>
#include "numpy_helper.h"
#include <cmath>

// helper function for comparing doubles
bool withinTolerance(double a, double b, double t = 0.01);


KMeans::KMeans(int n_clusters, int max_iter, int random_state) {
    this->n_clusters = n_clusters;
    this->max_iter = max_iter;
    
    if (random_state == -1)
        this->rand_seed = time(NULL);
    else
        this->rand_seed = random_state;
}

void KMeans::fit(double *invec, int n, int m) {
    
    std::srand(rand_seed);
    
    // allocate clusters
    for (int i = 0; i < n_clusters; i++)
        clusters.push_back(Cluster(invec, n, m));
    
    // initial random assignment of clusters
    for (int i = 0; i < n; i++) {
        int centroid = std::rand()%n_clusters;
        clusters[centroid].AddPoint(i);
    }
    
    // run until no change in clusters
    bool diff = true;
    
    for (int runs = 0; runs < max_iter; runs++) {
        
        diff = false;
        // calculate centroids
        for (int i = 0; i < n_clusters; i++) {
            diff |= clusters[i].CalcCentroid();
        }
        
        if (!diff)
            break;
        
        for (int i = 0; i < n_clusters; i++)
            clusters[i].ResetPoints();
        
        double *d;
        // reassign points
        for (int i = 0; i < n; i++) {
            d = invec + i * m;
           int minindex = NearestCluster(d, m);
            clusters[minindex].AddPoint(i);   
        }
    }
    
    for (int i = 0; i < n_clusters; i++) {
        std::cout << "Cluster #" << i << " centroid: ";
        clusters[i].PrintCentroid();
    }
    
    
}

int KMeans::NearestCluster(double *row, int n) {
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

std::vector<int> *KMeans::fit_predict(std::vector<std::vector<double> > *data) {
    ;
}

int KMeans::predict(double *row, int n) {
    return NearestCluster(row, n);
}


Cluster::Cluster(double *data, int rows, int columns) {
    this->data = data;
    this->rows = rows;
    this->columns = columns;
    this->n_attributes = columns;
    this->n_points = 0;
    
    for (int i = 0; i < columns; i++) {
        centroid.push_back(0);
    }
    
}

void Cluster::ResetCentroid() {

    centroid.clear();
    for (int i = 0; i < n_attributes; i++ )
        centroid.push_back(0);
}

void Cluster::AddPoint(int row) {
    
    points.push_back(row);
    n_points += 1;
}

bool Cluster::CalcCentroid() {
    
    
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

double Cluster::MinkowskiDist(double *d, int n) {
    double s = 0;
    
    if (n != n_attributes)
        return -1;
    
    for (int i = 0; i < n_attributes; i++) {
        s += std::pow(std::abs(centroid[i]-d[i]), 1.0/n);
    }
    
    return std::sqrt(s);
}

void Cluster::PrintCentroid() {
    std::cout << "[ " ;
    for (int i = 0; i < n_attributes; i++) 
        std::cout << centroid[i] << ", ";
    std::cout << "]" << std::endl;
}

void Cluster::ResetPoints() {
    points.clear();
    n_points = 0;
}


// helper function for comparing doubles
bool withinTolerance(double a, double b, double t) {
    return std::abs(a - b) <= t;
}
