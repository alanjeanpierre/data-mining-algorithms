import time
from sklearn import datasets, cluster
import numpy as np
import agnes
import kmeans
import dbscan

n = 3
print('Benchmarking KMeans, Agnes, and DBSCAN against their sklearn implementations')
print('Each time is an average of {:d} rounds against the same dataset'.format(n))
print('Benchmarks are represented by a ratio of reference time divided by implementation time')
print()
print('{:5s}\t{:10s}\t{:10s}\t{:10s}'.format('Npts', 'Kmeans', 'Agnes', 'DBSCAN'))
for num in range(100, 2000, 100):
    blobs, _ = datasets.make_blobs(num)


    #print("SKlearn KMeans implementation: ", end="")
    t_total = 0.0
    for i in range(n):
        c = cluster.KMeans(3)
        t0 = time.time()
        c.fit(blobs)
        t1 = time.time()
        t_total += (t1-t0)
    #print("{:.4f}".format(t_total/n))
    kref = t_total
    
    #print("C++ KMeans implementation: ", end="")
    t_total = 0.0
    for i in range(n):
        c = kmeans.KMeans(3)
        t0 = time.time()
        c.Fit(blobs)
        t1 = time.time()
        t_total += (t1-t0)
    #print("{:.4f}".format(t_total/n))
    kimpl = t_total
    
    #print("SKlearn AGNES implementation: ", end="")
    t_total = 0.0
    for i in range(n):
        c = cluster.AgglomerativeClustering(3)
        t0 = time.time()
        c.fit(blobs)
        t1 = time.time()
        t_total += (t1-t0)
    #print("{:.4f}".format(t_total/n))
    aref = t_total
    
    #print("C++ AGNES implementation: ", end="")
    t_total = 0.0
    for i in range(n):
        c = agnes.Agnes(3, 'single')
        t0 = time.time()
        c.Fit(blobs)
        t1 = time.time()
        t_total += (t1-t0)
    #print("{:.4f}".format(t_total/n))
    aimpl = t_total
    
    #print("SKlearn AGNES implementation: ", end="")
    t_total = 0.0
    for i in range(n):
        c = cluster.DBSCAN(1.5, 5)
        t0 = time.time()
        c.fit(blobs)
        t1 = time.time()
        t_total += (t1-t0)
    #print("{:.4f}".format(t_total/n))
    dref = t_total
    
    #print("C++ AGNES implementation: ", end="")
    t_total = 0.0
    for i in range(n):
        c = dbscan.DBSCAN(1.5, 5)
        t0 = time.time()
        c.Fit(blobs)
        t1 = time.time()
        t_total += (t1-t0)
    #print("{:.4f}".format(t_total/n))
    dimpl = t_total

    print('{:<5d}\t{:<10.4f}\t{:<10.4f}\t{:<10.4f}'.format(num, kref/kimpl, aref/aimpl, dref/dimpl))

