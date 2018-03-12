// send help
// https://docs.scipy.org/doc/numpy-1.13.0/reference/swig.interface-file.html
// http://scipy-cookbook.readthedocs.io/items/SWIG_NumPy_examples.html
// http://www.swig.org/Doc3.0/SWIGPlus.html
%module kmeans
%{
#define SWIG_FILE_WITH_INIT
#include "kmeans.h"
%}

%include "numpy.i"
%init %{
    import_array();
%}

%apply(double* IN_ARRAY2, int DIM1, int DIM2) {(double *invec, int n, int m)}

%include "kmeans.h"
