%module agnes
%{
#define SWIG_FILE_WITH_INIT
#include "agnes.h"
%}

%include "numpy.i"

%init %{
    import_array();
%}

%apply(double* IN_ARRAY2, int DIM1, int DIM2) {(double *arr, int rows, int cols)}
%apply(double* ARGOUT_ARRAY1, int DIM1) {(double *out, int n)}

%include "agnes.h"