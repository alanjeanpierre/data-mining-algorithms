from dbscan import DBSCAN
from sklearn import datasets
from sklearn import cluster
import numpy as np
import test_helper as t


def test_simple_single_dbscan():
    data = np.array([[1, 1],])
    impl = DBSCAN(1,1)

    impl.Fit(data)

    r_labels = np.array([0])
    i_labels = impl.GetLabels(data.shape[0])

    assert t.check_clusters(r_labels, i_labels, 1) == True

def test_simple_dual_dbscan():
    data = np.array([[1, 1],
                     [-1, -1]])
    impl = DBSCAN(0.5, 1)

    impl.Fit(data)

    r_labels = np.array([0,1])
    i_labels = impl.GetLabels(data.shape[0])

    assert t.check_clusters(r_labels, i_labels, 2) == True

def test_duplicate_dual_dbscan():
    data = np.array([[1,1], [1,1], [1,1], [1,1], [1,1], [1,1], [1,1], 
                    [-1,-1], [-1,-1], [-1,-1], [-1,-1], [-1,-1], [-1,-1], [-1,-1], ])
    impl = DBSCAN(0.1, 5)

    impl.Fit(data)

    r_labels = np.array([0,0,0,0,0,0,0,1,1,1,1,1,1,1])
    i_labels = impl.GetLabels(data.shape[0])

    assert t.check_clusters(r_labels, i_labels, 2) == True

def test_clear_blobs_dbscan():
    centers = ((-5, -5), (5, 5))
    data, _ = datasets.make_blobs(n_samples=100, centers=centers, cluster_std=0.1)
    
    ref = cluster.DBSCAN(0.6, 4)
    impl = DBSCAN(0.6, 4)
    impl.Fit(data)
    ref.fit(data)
    r_labels = ref.labels_
    i_labels = impl.GetLabels(data.shape[0])

    assert t.check_clusters(r_labels, i_labels, 2) == True

def test_large_dbscan():
    data, r_labels = datasets.make_blobs(n_samples=1000, centers=1)

    impl = DBSCAN(0.6, 4)
    ref = cluster.DBSCAN(0.6, 4)
    impl.Fit(data)
    ref.fit(data)
    r_labels = ref.labels_
    i_labels = impl.GetLabels(data.shape[0])

    assert t.check_clusters(r_labels, i_labels, 1) == True

    
def test_double_fit_dbscan():
    data1, r_labels1 = datasets.make_blobs(n_samples=288, centers=6, cluster_std=0.2, random_state=31)
    data2, r_labels2 = datasets.make_blobs(n_samples=288, centers=6, cluster_std=0.2, random_state=31)

    impl = DBSCAN(0.6, 4)
    impl.Fit(data1)

    i_labels1 = impl.GetLabels(data1.shape[0])

    impl.Fit(data2)
    i_labels2 = impl.GetLabels(data2.shape[0])

    t2 = t.check_clusters_with_allowance(i_labels2, i_labels1, 6, .01)

    assert t2 == True

def test_dimensionality_dbscan():
    data, r_labels = datasets.make_blobs(n_samples=288, n_features=16, cluster_std=0.2, random_state=31)

    impl = DBSCAN(0.6, 4)
    impl.Fit(data)
    ref = cluster.DBSCAN(0.6, 4)
    ref.fit(data)

    assert True == t.check_clusters(ref.labels_, impl.GetLabels(data.shape[0]), 3)

def test_epsilon_dbscan(dbscanparams):
    data, _ = datasets.make_blobs(n_samples=512, n_features=2, random_state=73)

    impl = DBSCAN(dbscanparams[0], dbscanparams[1])
    ref = cluster.DBSCAN(dbscanparams[0], dbscanparams[1])
    impl.Fit(data)
    ref.fit(data)

    i_labels = impl.GetLabels(data.shape[0])
    r_labels = ref.labels_
    assert np.unique(i_labels).shape[0] == np.unique(r_labels).shape[0]
    assert True == t.check_clusters(r_labels, i_labels, np.unique(i_labels).shape[0])