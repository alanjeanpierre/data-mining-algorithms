import agnes
from sklearn import datasets
import numpy as np
import test_helper as t


def test_simple_single_agnes(single):
    data = np.array([[1, 1],])
    impl = single

    impl.Fit(data)

    r_labels = np.array([0])
    i_labels = impl.GetLabels(data.shape[0])

    assert t.check_clusters(r_labels, i_labels, 1) == True

def test_simple_dual_agnes(dual):
    data = np.array([[1, 1],
                     [-1, -1]])
    impl = dual

    impl.Fit(data)

    r_labels = np.array([0,1])
    i_labels = impl.GetLabels(data.shape[0])

    assert t.check_clusters(r_labels, i_labels, 2) == True

def test_duplicate_dual_agnes(dual):
    data = np.array([[1,1], [1,1], [1,1], [1,1], [1,1], [1,1], [1,1], 
                    [-1,-1], [-1,-1], [-1,-1], [-1,-1], [-1,-1], [-1,-1], [-1,-1], ])
    impl = dual

    impl.Fit(data)

    r_labels = np.array([0,0,0,0,0,0,0,1,1,1,1,1,1,1])
    i_labels = impl.GetLabels(data.shape[0])

    assert t.check_clusters(r_labels, i_labels, 2) == True

def test_clear_blobs_agnes(dual):
    centers = ((-5, -5), (5, 5))
    data, r_labels = datasets.make_blobs(n_samples=100, centers=centers)
    impl = dual

    impl.Fit(data)
    i_labels = impl.GetLabels(data.shape[0])

    assert t.check_clusters(r_labels, i_labels, 2) == True

def test_double_fit_agnes():
    data1, r_labels1 = datasets.make_blobs(n_samples=288, centers=6, cluster_std=0.2, random_state=31)
    data2, r_labels2 = datasets.make_blobs(n_samples=288, centers=6, cluster_std=0.2, random_state=79)

    impl = agnes.Agnes(6, 'wards')
    impl.Fit(data1)

    t1 = t.check_clusters(r_labels1, impl.GetLabels(data1.shape[0]), 6,)
    
    impl.Fit(data2)
    t2 = t.check_clusters(r_labels2, impl.GetLabels(data2.shape[0]), 6)

    assert t1 == True and t2 == True

def test_dimensionality_agnes(triple):
    data, r_labels = datasets.make_blobs(n_samples=60, n_features=16, cluster_std=0.2, random_state=31)

    impl = triple
    impl.Fit(data)

    assert True == t.check_clusters(r_labels, impl.GetLabels(data.shape[0]), 3)

    