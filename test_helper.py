import numpy as np

def build_k_by_k_matrix(k1, k2, n_clusters):
    """Builds k by k matrix from input arrays
    Purpose is so map from 1 arbitrary classification to another
    ie, if k1 returns [1, 1, 2, 1] and 
    k2 return [2, 2, 1, 2], they're identical. a kxk matrix would look like

    k1\k2
       1   2
    1  0   3
    2  1   0

    each row/column has exactly 1 nonzero element, IFF they cluster the same
    """

    m = np.zeros((n_clusters, n_clusters))
    for class1, class2 in zip(k1, k2):
        m[class1][class2] += 1
    print(m)
    return m    

def check_clusters(ref, impls, n_clusters):
    m = build_k_by_k_matrix(ref, impls, n_clusters)
    nonzero = np.count_nonzero(m, axis=0)
    return np.array_equal(nonzero, np.ones(nonzero.size))