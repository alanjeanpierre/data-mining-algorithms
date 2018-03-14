import kmeans
import numpy as np
from sklearn.cluster import KMeans

k = kmeans.KMeans(3,10,1)
a = np.arange(1000).reshape(250,4)
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
