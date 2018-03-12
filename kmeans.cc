#include "kmeans.h"
#include <iostream>

KMeans::KMeans(int n_clusters, int max_iter, int random_state) {
    this->n_clusters = n_clusters;
    this->max_iter = max_iter;
}

void KMeans::fit(double *invec, int n, int m) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            
            // need to go by m, not n
            int index = i*m + j;
            std::cerr << invec[index] << " ";
        }
        std::cerr << std::endl;
    }
}

std::vector<int> *KMeans::fit_predict(std::vector<std::vector<double> > *data) {
    ;
}

std::vector<int> *KMeans::predict(std::vector<std::vector<double> > *data) {
    ;
}
