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
    i_labels = np.array([impl.Predict(label) for label in data])

    assert t.check_clusters(r_labels, i_labels, 1) == True

def test_simple_dual():
    data = np.array([[1, 1],
                     [-1, -1]])
    ref = cluster.KMeans(2, random_state=rstate)
    impl = kmeans.KMeans(2, max_iter, rstate)

    ref.fit(data)
    impl.Fit(data)

    r_labels = ref.labels_
    i_labels = np.array([impl.Predict(label) for label in data])

    assert t.check_clusters(r_labels, i_labels, 2) == True

def test_duplicate_dual():
    data = np.array([[1,1], [1,1], [1,1], [1,1], [1,1], [1,1], [1,1], 
                    [-1,-1], [-1,-1], [-1,-1], [-1,-1], [-1,-1], [-1,-1], [-1,-1], ])
    ref = cluster.KMeans(2, random_state=rstate)
    impl = kmeans.KMeans(2, max_iter, rstate)

    ref.fit(data)
    impl.Fit(data)

    r_labels = ref.labels_
    i_labels = np.array([impl.Predict(label) for label in data])

    assert t.check_clusters(r_labels, i_labels, 2) == True

def test_clear_blobs():
    centers = ((-5, -5), (5, 5))
    data, r_labels = datasets.make_blobs(n_samples=100, centers=centers)
    
    impl = kmeans.KMeans(2, max_iter, rstate)
    impl.Fit(data)
    i_labels = np.array([impl.Predict(label) for label in data])

    assert t.check_clusters(r_labels, i_labels, 2) == True