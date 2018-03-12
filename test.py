import kmeans
import numpy as np

k = kmeans.KMeans(1,1,1)
a = np.arange(20).reshape(5,4)
print(a)
print('c++ version:')
k.fit(a)
