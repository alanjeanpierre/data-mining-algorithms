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

def test_unclear_blobs():
    data, r_labels = datasets.make_blobs(n_samples=144)

    impl = agnes.Agnes(3, 'single')
    impl.Fit(data)

    assert t.check_clusters_with_allowance(r_labels, impl.GetLabels(data.shape[0]), 3, .1) == True

def test_linkage():
    data, r_labels = datasets.make_blobs(n_samples=144)

    single = agnes.Agnes(3, 'single')
    complete = agnes.Agnes(3, 'complete')
    average = agnes.Agnes(3, 'average')

    single.Fit(data)
    complete.Fit(data)
    average.Fit(data)

    ts = t.check_clusters_with_allowance(r_labels, single.GetLabels(data.shape[0]), 3, .1)
    tc = t.check_clusters_with_allowance(r_labels, complete.GetLabels(data.shape[0]), 3, .1)
    ta = t.check_clusters_with_allowance(r_labels, average.GetLabels(data.shape[0]), 3, .1)


    assert ts == True and tc == True and ta == True

def test_double_fit():
    data1, r_labels1 = datasets.make_blobs(n_samples=288, centers=6, random_state=31)
    data2, r_labels2 = datasets.make_blobs(n_samples=288, centers=6, random_state=73)

    impl = agnes.Agnes(6, 'complete')
    impl.Fit(data1)

    t1 = t.check_clusters_with_allowance(r_labels1, impl.GetLabels(data1.shape[0]), 6, .1)
    
    impl.Fit(data2)
    t2 = t.check_clusters_with_allowance(r_labels2, impl.GetLabels(data2.shape[0]), 6, .1)

    assert t1 == True and t2 == True
