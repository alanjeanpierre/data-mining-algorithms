#include "kmeans.h"
#include <iostream>
#include <ctime>
#include <cstdlib>
#include "numpy_helper.h"
#include <cmath>
#include <map>

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
    
    data = new std::vector<std::vector<double> >();
}

KMeans::~KMeans() {
    delete data;
    data = NULL;
    
}

void KMeans::CopyData(double *invec, int n, int m) {

    data->resize(n);
    for (int i = 0; i < n; i++) {
        double *d = invec + i*n_attributes;
        #ifdef _DEBUG
        std::cerr << "Point " << i << ": <";
        #endif
        std::vector<double> t;
        for (int j = 0; j < n_attributes; j++) {
            t.push_back(d[j]);
            #ifdef _DEBUG
            std::cerr << d[j] << ",";
            #endif
        }

        #ifdef _DEBUG
        std::cerr << ">" << std::endl;
        #endif
        data->at(i) = t;;
    }

}

void KMeans::PrintClusters() {
    for (int i = 0; i < n_clusters; i++) {
        std::cerr << "Points in Cluster #" << i << std::endl;
        std::vector<int> *pts = clusters[i].GetPoints();
        for (unsigned int j = 0; j < pts->size(); j++) {
            std::cerr << "<";
            for (int k = 0; k < n_attributes; k++) {
                std::cerr << data->at(pts->at(j))[k] << ",";
            }
            std::cerr << ">" << std::endl;
        }
        std::cerr << std::endl;
    }
}

void KMeans::Fit(double *invec, int n, int m) {
    /*
     * Let 
     *      n = num points
     *      k = num clusters
     *      d = dimensionality of the data
     *      i = num iterations 
     * 
     * Fit = O(kdni)
     */
    
    std::srand(rand_seed);
    n_attributes = m;
    
    // copy data to internal data structure
    CopyData(invec, n, m);  // O(n)

    // allocate clusters
    for (int i = 0; i < n_clusters; i++)
        clusters.push_back(Cluster(data, n, n_attributes));
    
    int i;

    // initial diff assignment of clusters
    // this is to ensure there are AT LEAST n_clusters
    // and aren't randomly assigned to the same one
    for (i = 0; i < n_clusters; i++) {
        #ifdef _DEBUG
        std::cerr << "Deterministically assigning point " << i << " to cluster " << i << std::endl;
        #endif
        clusters[i].AddPoint(i);
    }

    // initial random assignment of remaining clusters
    // starting where prev assignment left off
    for (; i < n; i++) { // O(n)
        int centroid = std::rand()%n_clusters;
        clusters[centroid].AddPoint(i);
        #ifdef _DEBUG
        std::cerr << "Randomly assigning point " << i << " to cluster " << centroid << std::endl;
        #endif
    }

    #ifdef _DEBUG
    PrintClusters();
    #endif
    
    // run until no change in clusters
    bool diff = true;
    #ifdef _DEBUG
    int iter = 1;
    #endif
    // O(i) iterations with O(n) and O(cdn) per iteration
    // O(kdni)
    for (int runs = 0; diff && runs < max_iter; runs++) {
        #ifdef _DEBUG
        std::cerr << "Iteration #" << iter << std::endl;
        #endif

        diff = false;
        // calculate centroids
        for (int i = 0; i < n_clusters; i++) { // O(k) of O(n) = O(kn)
            #ifdef _DEBUG
            std::cerr << "New centroid for cluster #" << i << " <";
            #endif
            diff |= clusters[i].CalcCentroid(); // O(n)
        }
        
        for (int i = 0; i < n_clusters; i++)
            clusters[i].ResetPoints();
        
        // reassign points
        for (int i = 0; i < n; i++) { // O(n) iterations of O(kd) each = O(cdn)
            std::vector<double> *d = &(data->at(i));
            int minindex = NearestCluster(d); // O(kd)
            clusters[minindex].AddPoint(i);   
            
            #ifdef _DEBUG
            std::cerr << "Adding point " << i << " to cluster " << minindex << std::endl;
            #endif
        }

        #ifdef _DEBUG
        PrintClusters();
        iter++;
        #endif
    }   
}

int KMeans::NearestCluster(std::vector<double> *datapoint) {

    if (datapoint->size() != (unsigned int)n_attributes) {
        std::cerr << "Error: num input attributes (" << datapoint->size()
                  << ") doesn't match num cluster attributes (" 
                  << n_attributes << ")" << std::endl;
        return 0;
    }

     // nearest centroid
    double mindist = 1 << 30; // hsould set to some max double?
    int minindex = 0;
    for (int c = 0; c < n_clusters; c++) {
        double dist = clusters[c].MinkowskiDist(datapoint, 2);
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
    std::vector<double> d;
    for (int i = 0; i < n; i++)
        d.push_back(row[i]);
    return NearestCluster(&d);
}

void KMeans::GetLabels(int *out, int n) {
    std::map<int, int> *clustermap = new std::map<int, int>(); 
    for (int i = 0; i < n_clusters; i++) {
        std::vector<int> *pts = clusters[i].GetPoints();
        for (unsigned int j = 0; j < pts->size(); j++) {
            clustermap->insert(std::pair<int, int>(pts->at(j), i));
        }
    }

    for (int i = 0; i < n; i++) {
        out[i] = clustermap->find(i)->second;
    }
}


KMeans::Cluster::Cluster(std::vector<std::vector<double> > *data, int rows, int columns) {
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
    
    if (n_points == 0)
        return false;
    
    double *old = new double[n_attributes];
    for (int i = 0; i < n_attributes; i++)
        old[i] = centroid[i];
    
    ResetCentroid();
    
    // sum points
    for (int i = 0; i < n_points; i++) {
        for (int j = 0; j < n_attributes; j++) {
            centroid[j] += data->at(points[i])[j];
        }
    }
    
    bool diff = false;
    // average
    for (int i = 0; i < n_attributes; i++) {
        centroid[i] /= n_points;
        diff |= !withinTolerance(centroid[i], old[i], 0.1);
    }    

    delete old;


    #ifdef _DEBUG
    for (int i = 0; i < n_attributes; i++)
        std::cerr << centroid[i] << ", ";
    std::cerr << ">" << std::endl;
    #endif

    return diff;
}

double KMeans::Cluster::MinkowskiDist(std::vector<double> *datapoint, int n) {
    double s = 0;
    
    for (int i = 0; i < n_attributes; i++) {
        s += std::pow(std::abs(centroid[i]-datapoint->at(i)), n);
    }
    
    return std::pow(s, 1.0/n);
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

std::vector<int> *KMeans::Cluster::GetPoints() {
    return &points; 
}

// helper function for comparing doubles
bool withinTolerance(double a, double b, double t) {
    return std::fabs(a - b) <= ((std::fabs(a) < std::fabs(b)) ? std::fabs(b) : std::fabs(a)) * t;
}

