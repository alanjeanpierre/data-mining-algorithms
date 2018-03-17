import time
from sklearn import datasets, cluster
import numpy as np
import agnes
import kmeans

n = 10

for i in [100, 500, 1000, 2000]:
    print('n = {}'.format(i))
    blobs, _ = datasets.make_blobs(i)


    print("SKlearn KMeans implementation: ", end="")
    t_total = 0.0
    for i in range(n):
        c = cluster.KMeans(3)
        t0 = time.time()
        c.fit(blobs)
        t1 = time.time()
        t_total += (t1-t0)
    print("{:.4f}".format(t_total/n))
    
    print("C++ KMeans implementation: ", end="")
    t_total = 0.0
    for i in range(n):
        c = kmeans.KMeans(3)
        t0 = time.time()
        c.Fit(blobs)
        t1 = time.time()
        t_total += (t1-t0)
    print("{:.4f}".format(t_total/n))
    
    print("SKlearn AGNES implementation: ", end="")
    t_total = 0.0
    for i in range(n):
        c = cluster.AgglomerativeClustering(3)
        t0 = time.time()
        c.fit(blobs)
        t1 = time.time()
        t_total += (t1-t0)
    print("{:.4f}".format(t_total/n))
    
    print("C++ AGNES implementation: ", end="")
    t_total = 0.0
    for i in range(n):
        c = agnes.Agnes(3, 'single')
        t0 = time.time()
        c.Fit(blobs)
        t1 = time.time()
        t_total += (t1-t0)
    print("{:.4f}".format(t_total/n))

    print()

