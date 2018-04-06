import time
import sys
from sklearn import datasets, cluster
import numpy as np
import agnes
import kmeans
import dbscan
import platform, os, subprocess

opsys = platform.system()
osver = platform.release()
gccver = subprocess.check_output(['g++', '--version']).decode('utf-8').split('\n')[0]
pyver = platform.python_version()

n = 20
if '-n' in sys.argv:
    n = int(sys.argv[sys.argv.index('-n')+1])

r = list()
if '-r' in sys.argv:
    for i in sys.argv[sys.argv.index('-r')+1:]:
        r.append(int(i))

print('Running on {} v. {}'.format(opsys, osver))
print('Algorithms were compiled with {}'.format(gccver))
print('Running on Python version {}'.format(pyver))
print('Benchmarking KMeans, Agnes, and DBSCAN implementations \nagainst their sklearn reference implementations')
print('Each time is an average of {:d} rounds against the same dataset in seconds'.format(n))
print()
print('{:5s}\t{:10s}\t{:10s}\t{:10s}\t{:10s}\t{:10s}\t{:10s}'.format('Npts', 'KmeansSK', 'KmeansC++', 'AgnesSK', 'AgnesC++', 'DBSCANSK', 'DBSCANC++'))

for num in r:
    blobs, _ = datasets.make_blobs(num)


    
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

    print('{:<5d}\t{:<10.4f}\t{:<10.4f}\t{:<10.4f}\t{:<10.4f}\t{:<10.4f}\t{:<10.4f}'.format(num, kref, kimpl, aref, aimpl, dref, dimpl))

