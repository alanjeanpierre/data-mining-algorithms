%module dbscan
%{
#define SWIG_FILE_WITH_INIT
#include "dbscan.h"
%}

%include "numpy.i"

%init %{
    import_array();
%}

%apply(double* IN_ARRAY2, int DIM1, int DIM2) {(double *arr, int rows, int cols)}
%apply(int* ARGOUT_ARRAY1, int DIM1) {(int *out, int n)}

%include "dbscan.h"