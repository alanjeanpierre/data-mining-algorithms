import kmeans
import numpy as np
import sys
from sklearn.cluster import KMeans

k = kmeans.KMeans(3,10,1)
a = np.arange(20).reshape(5,4) + 100
b = np.arange(20).reshape(5,4)
c = np.arange(20).reshape(5,4) - 100

a = np.concatenate((a, b, c))

print('np array:')
print(a[:5])
print('...')
print(a[-5:])

skk = KMeans(3)
skk.fit(a)
print('sklearns centroids:')
print(skk.cluster_centers_)

print('fitting c++ cluster')
k.fit(a)

print('trying to get clusters using invalid array')
clusters = np.ndarray((1,9), dtype=float)
k.GetClusters(clusters)


print('c++ centroids:')
clusters = np.ndarray((3,4), dtype=float)
k.GetClusters(clusters)
print(clusters)

sys.stderr.write("Importing\n")
import agnes
sys.stderr.write("Done import\n")
sys.stderr.write("Initting\n")
ag = agnes.Agnes(5)
sys.stderr.write("Done init\n")
sys.stderr.write("Fitting\n")
ag.Fit(a)
sys.stderr.write("Done fitting\n")
sys.stderr.write("Cluster representation\n")
clusters = np.ndarray((5,1), dtype=float)

print(len(a))

print(ag.GetLabels(len(a)))

from sklearn import datasets
blobs, _ = datasets.make_blobs(n_samples=300, random_state=10)

ag = agnes.Agnes(3)
skk.fit(blobs)
print("sklearn clusters")
print(skk.labels_[:30])
ag.Fit(blobs)
print('agnes clusters')
print(ag.GetLabels(len(blobs)).astype(np.int)[:30])