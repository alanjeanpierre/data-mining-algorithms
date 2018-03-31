import time
import sys
from sklearn import datasets, cluster
import numpy as np
import agnes
import kmeans
import dbscan

n = 20
if '-n' in sys.argv:
    n = int(sys.argv[sys.argv.index('-n')+1])

increment = 100
if '-i' in sys.argv:
    increment = int(sys.argv[sys.argv.index('-i')+1])

max_iter = 2000
if '-r' in sys.argv:
    max_iter = int(sys.argv[sys.argv.index('-r')+1])

ref = False
if '-p' in sys.argv:
    ref = True

print('Benchmarking KMeans, Agnes, and DBSCAN implementations \nagainst their sklearn reference implementations')
print('Each time is an average of {:d} rounds against the same dataset'.format(n))
if ref:
    print('Times are represented as a ratio of ref/impl')
print()
print('{:5s}\t{:10s}\t{:10s}\t{:10s}'.format('Npts', 'Kmeans', 'Agnes', 'DBSCAN'))
for num in range(100, max_iter, increment):
    blobs, _ = datasets.make_blobs(num)


    if ref:
        t_total = 0.0
        for i in range(n):
            c = cluster.KMeans(3)
            t0 = time.time()
            c.fit(blobs)
            t1 = time.time()
            t_total += (t1-t0)
        kref = t_total/n
    
    t_total = 0.0
    for i in range(n):
        c = kmeans.KMeans(3)
        t0 = time.time()
        c.Fit(blobs)
        t1 = time.time()
        t_total += (t1-t0)
    kimpl = t_total/n
    
    if ref:
        t_total = 0.0
        for i in range(n):
            c = cluster.AgglomerativeClustering(3)
            t0 = time.time()
            c.fit(blobs)
            t1 = time.time()
            t_total += (t1-t0)
        aref = t_total/n
    
    t_total = 0.0
    for i in range(n):
        c = agnes.Agnes(3, 'wards')
        t0 = time.time()
        c.Fit(blobs)
        t1 = time.time()
        t_total += (t1-t0)
    aimpl = t_total/n
    
    if ref:
        t_total = 0.0
        for i in range(n):
            c = cluster.DBSCAN(1.5, 5)
            t0 = time.time()
            c.fit(blobs)
            t1 = time.time()
            t_total += (t1-t0)
        dref = t_total/n
    
    
    t_total = 0.0
    for i in range(n):
        c = dbscan.DBSCAN(1.5, 5)
        t0 = time.time()
        c.Fit(blobs)
        t1 = time.time()
        t_total += (t1-t0)
    dimpl = t_total/n
        
    if ref:
        print('{:<5d}\t{:<10.4f}\t{:<10.4f}\t{:<10.4f}'.format(num, kref/kimpl, aref/aimpl, dref/dimpl))
    else:
        print('{:<5d}\t{:<10.4f}\t{:<10.4f}\t{:<10.4f}'.format(num, kimpl, aimpl, dimpl))

