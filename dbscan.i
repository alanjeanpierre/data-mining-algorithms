%module dbscan
%{
#include "dbscan.h"
%}


%apply(double* IN_ARRAY2, int DIM1, int DIM2) {(double *arr, int rows, int cols)}
%apply(int* ARGOUT_ARRAY1, int DIM1) {(int *out, int n)}