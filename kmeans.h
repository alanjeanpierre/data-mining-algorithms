#ifndef _K_MEANS
#define _K_MEANS

#include <vector>

class KMeans {
    private:
        std::vector<std::vector<double> > *data;
        int n_clusters;
        int max_iter;
        std::vector<int> labels;

    public:
        KMeans(int n_clusters=8, int max_iter=300, int random_state=-1);
        void fit(double *invec, int n, int m);
        
        // can't use vectors
        std::vector<int> *fit_predict(std::vector<std::vector<double> > *data);
        std::vector<int> *predict(std::vector<std::vector<double> > *data);

};

#endif
