[ -f ./numpy.i ] && echo "numpy.i already here, good" || wget https://raw.githubusercontent.com/numpy/numpy/master/tools/swig/numpy.i
[ -f ./pyfragments.swg ] && echo "fragments already here, good" || wget https://raw.githubusercontent.com/numpy/numpy/master/tools/swig/pyfragments.swg
swig -c++ -python agnes.i
swig -c++ -python dbscan.i
swig -c++ -python kmeans.i
python3 setup.py build_ext --inplace
