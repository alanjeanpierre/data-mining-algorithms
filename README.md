# Project Title:	We’re doing it in C++

# Team Name:
* Lexi Vessels
* Alan Jeanpierre
* Eduardo Mata
* Hector Flores

# Proposed Project
We will implement the K-means, Agnes, and DBSCAN clustering algorithms. We will do it in C++11 without any external libraries for portability. We will test our algorithms against both sk-learn and WEKA’s implementations. 

# Datasets
* Primarily we will demonstrate the correctness with generated datasets from sk-learn
* We will use simple ‘hello world’ type of datasets, such as the Iris dataset or BUPA dataset, purely to demonstrate its applicability
  * [Iris Dataset](http://archive.ics.uci.edu/ml/datasets/Iris)
  * [BUPA Dataset](ftp://ftp.ics.uci.edu/pub/machine-learning-databases/liver-disorders)

# Requirements
* Source installation requires:
  * [Make](https://www.gnu.org/software/make/) 
  * [Wget](https://www.gnu.org/software/wget/)
  * [SWIG](http://www.swig.org/) 
  * [GCC](https://gcc.gnu.org/) or similar C++11 compliant compiler
  * [Python3](https://www.python.org/downloads/) 
  * [Numpy](http://www.numpy.org/) 
* To run the example jupyter notebook, the following packages are required:
  * [Jupyter Notebook](http://jupyter.org/) 
  * [scikit-learn](http://scikit-learn.org/stable/)
  * [NumPy](http://www.numpy.org/)
  * [Matplotlib](https://matplotlib.org/) 
* To run the tests, the only dependencies are:
  * [Pytest](https://docs.pytest.org/en/latest/) 
  * [scikit-learn](http://scikit-learn.org/stable/)
  * [NumPy](http://www.numpy.org/)

# Installation
* On *NIX Simply run `make` in the root directory. 
  * This will compile the files an generate a python file for import in the current directory. This option does **not** install to your python import location
* On Windows, run the included install.bat file
  * This option installs the python files to your import location
* There are prebuilt binaries and python scripts available for some common platforms in `bin.tar.gz`

# Testing
* Our software comes with tests in the root directory
* Simply run with `pytest`
* During compilation, passing the environment variable `export DEBUG=1` will enable debug output on stderr

# Examples
* Please refer to the `example.ipynb` file for comprehensive examples