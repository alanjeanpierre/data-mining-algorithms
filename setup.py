#!~/usr/bin/env python3

"""
setup.py file for SWIG c++ files
"""


from distutils.core import setup, Extension


import numpy
kmeans = Extension('_kmeans',
                    sources=['kmeans_wrap.cxx', 'kmeans.cc'], 
                    include_dirs=[numpy.get_include()],)
agnes = Extension('_agnes',
                  sources=['agnes_wrap.cxx', 'agnes.cc'], 
                  include_dirs=[numpy.get_include()], )
dbscan = Extension('_dbscan', 
                   sources=['dbscan_wrap.cxx', 'dbscan.cc'], 
                   include_dirs=[numpy.get_include()], )

setup( name = 'diycluster',
       version = '0.1',
       description = """Implementations in C++ of k-means, agnes, and DBSCAN""",
       ext_modules = [kmeans, agnes, dbscan],
       py_modules = ['kmeans', 'agnes', 'dbscan'],
       
    )
