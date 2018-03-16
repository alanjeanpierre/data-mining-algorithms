import kmeans
from sklearn import datasets
from sklearn import cluster
import numpy as np

max_iter = 300
rstate = 31

def build_k_by_k_matrix(k1, k2):
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

    m = np.zeros((k1.size, k2.size))
    for class1, class2 in zip(k1, k2):
        m[class1][class2] += 1
    return m    

def check_clusters(ref, impls):
    m = build_k_by_k_matrix(ref, impls)
    nonzero = np.count_nonzero(m, axis=0)
    return np.array_equal(nonzero, np.ones(nonzero.size))

def test_simple_single():
    data = np.array([[1, 1],])
    ref = cluster.KMeans(1, random_state=rstate)
    impl = kmeans.KMeans(1, max_iter, rstate)

    ref.fit(data)
    impl.Fit(data)

    r_labels = ref.labels_
    i_labels = np.array([impl.Predict(label) for label in data])

    assert check_clusters(r_labels, i_labels) == True

def test_simple_dual():
    data = np.array([[1, 1],
                     [-1, -1]])
    ref = cluster.KMeans(2, random_state=rstate)
    impl = kmeans.KMeans(2, max_iter, rstate)

    ref.fit(data)
    impl.Fit(data)

    r_labels = ref.labels_
    i_labels = np.array([impl.Predict(label) for label in data])

    assert check_clusters(r_labels, i_labels) == True
