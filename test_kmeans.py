import kmeans
from sklearn import datasets
from sklearn import cluster
import numpy as np
import test_helper as t

max_iter = 300
rstate = 31

def test_simple_single():
    data = np.array([[1, 1],])
    ref = cluster.KMeans(1, random_state=rstate)
    impl = kmeans.KMeans(1, max_iter, rstate)

    ref.fit(data)
    impl.Fit(data)

    r_labels = ref.labels_
    i_labels = impl.GetLabels(data.shape[0])

    assert t.check_clusters(r_labels, i_labels, 1) == True

def test_simple_dual():
    data = np.array([[1, 1],
                     [-1, -1]])
    ref = cluster.KMeans(2, random_state=rstate)
    impl = kmeans.KMeans(2, max_iter, rstate)

    ref.fit(data)
    impl.Fit(data)

    r_labels = ref.labels_
    i_labels = impl.GetLabels(data.shape[0])

    assert t.check_clusters(r_labels, i_labels, 2) == True

def test_duplicate_dual():
    data = np.array([[1,1], [1,1], [1,1], [1,1], [1,1], [1,1], [1,1], 
                    [-1,-1], [-1,-1], [-1,-1], [-1,-1], [-1,-1], [-1,-1], [-1,-1], ])
    ref = cluster.KMeans(2, random_state=rstate)
    impl = kmeans.KMeans(2, max_iter, rstate)

    ref.fit(data)
    impl.Fit(data)

    r_labels = ref.labels_
    i_labels = impl.GetLabels(data.shape[0])

    assert t.check_clusters(r_labels, i_labels, 2) == True

def test_clear_blobs():
    centers = ((-5, -5), (5, 5))
    data, r_labels = datasets.make_blobs(n_samples=100, centers=centers)
    
    impl = kmeans.KMeans(2, max_iter, rstate)
    impl.Fit(data)
    i_labels = impl.GetLabels(data.shape[0])

    assert t.check_clusters(r_labels, i_labels, 2) == True

def test_large():
    data, r_labels = datasets.make_blobs(n_samples=1000, centers=1)

    impl = kmeans.KMeans(1, max_iter, rstate)
    impl.Fit(data)
    i_labels = impl.GetLabels(data.shape[0])

    assert t.check_clusters(r_labels, i_labels, 1) == True

    
def test_double_fit():
    data1, r_labels1 = datasets.make_blobs(n_samples=288, centers=6, random_state=31)
    data2, r_labels2 = datasets.make_blobs(n_samples=288, centers=6, random_state=73)

    impl = kmeans.KMeans(6)
    impl.Fit(data1)

    t1 = t.check_clusters_with_allowance(r_labels1, impl.GetLabels(data1.shape[0]), 6, .1)
    
    impl.Fit(data2)
    t2 = t.check_clusters_with_allowance(r_labels2, impl.GetLabels(data2.shape[0]), 6, .1)

    assert t1 == True and t2 == True
