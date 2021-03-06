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
%apply(double* IN_ARRAY1, int DIM1) {(double *row, int n)}
%apply(double* INPLACE_ARRAY2, int DIM1, int DIM2) {(double *arr, int rows, int cols)}
%apply(double* ARGOUT_ARRAY1, int DIM1, int nth) {(double *arr, int cols, int nth)}
%apply(int* ARGOUT_ARRAY1, int DIM1) {(int *out, int n)}

%include "kmeans.h"
