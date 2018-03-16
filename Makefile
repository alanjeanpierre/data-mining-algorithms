
all: numpy.i pyfragments.swg kmeans.py agnes.py dbscan.py
	python3 setup.py build_ext --inplace

kmeans.py: kmeans.cc
	swig -c++ -python kmeans.i
	
agnes.py: agnes.cc
	swig -c++ -python agnes.i
	
dbscan.py: dbscan.cc
	swig -c++ -python dbscan.i

numpy.i:
	wget https://raw.githubusercontent.com/numpy/numpy/master/tools/swig/numpy.i

pyfragments.swg:
	wget https://raw.githubusercontent.com/numpy/numpy/master/tools/swig/pyfragments.swg	

clean:
	rm kmeans.py agnes.py dbscan.py *.cxx *.so