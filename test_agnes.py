import agnes
from sklearn import datasets
import numpy as np
import test_helper as t


def test_simple_single():
    data = np.array([[1, 1],])
    impl = agnes.Agnes(1, 'single')

    impl.Fit(data)

    r_labels = np.array([0])
    i_labels = impl.GetLabels(data.shape[0])

    assert t.check_clusters(r_labels, i_labels, 1) == True

def test_simple_dual():
    data = np.array([[1, 1],
                     [-1, -1]])
    impl = agnes.Agnes(2, 'single')

    impl.Fit(data)

    r_labels = np.array([0,1])
    i_labels = impl.GetLabels(data.shape[0])

    assert t.check_clusters(r_labels, i_labels, 2) == True

def test_duplicate_dual():
    data = np.array([[1,1], [1,1], [1,1], [1,1], [1,1], [1,1], [1,1], 
                    [-1,-1], [-1,-1], [-1,-1], [-1,-1], [-1,-1], [-1,-1], [-1,-1], ])
    impl = agnes.Agnes(2, 'single')

    impl.Fit(data)

    r_labels = np.array([0,0,0,0,0,0,0,1,1,1,1,1,1,1])
    i_labels = impl.GetLabels(data.shape[0])

    assert t.check_clusters(r_labels, i_labels, 2) == True

def test_clear_blobs():
    centers = ((-5, -5), (5, 5))
    data, r_labels = datasets.make_blobs(n_samples=100, centers=centers)
    
    impl = agnes.Agnes(2, 'single')

    impl.Fit(data)
    i_labels = impl.GetLabels(data.shape[0])

    assert t.check_clusters(r_labels, i_labels, 2) == True

def test_large():
    data, r_labels = datasets.make_blobs(n_samples=2000, centers=1)

    impl = agnes.Agnes(1, 'single')
    impl.Fit(data)
    i_labels = impl.GetLabels(data.shape[0])

    assert t.check_clusters(r_labels, i_labels, 1) == True
    