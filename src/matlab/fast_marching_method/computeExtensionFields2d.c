/*
 * File:        computeExtensionFields2d.c
 * Copyright:   (c) 2005-2008 Kevin T. Chu and Masa Prodanovic
 * Revision:    $Revision: 1.14 $
 * Modified:    $Date$
 * Description: MATLAB MEX-file for using the fast marching method to
 *              compute extension fields for 2d level set functions
 */

/*===========================================================================
 *
 * computeExtensionFields2d() computes a distance from an 
 * arbitrary level set function using the Fast Marching Method.
 * 
 * Usage: [distance_function, extension_fields] = ...
 *        computeExtensionFields2d(phi, source_fields, dX, ...
 *                                 mask, ...
 *                                 spatial_discretization_order)
 *
 * Arguments:
 * - phi:                           level set function to use in 
 *                                  computing distance function
 * - source_fields:                 field variables that are to
 *                                  be extended off of the zero
 *                                  level set
 * - dX:                            array containing the grid spacing
 *                                  in each coordinate direction
 * - mask:                          mask for domain of problem;
 *                                  grid points outside of the domain
 *                                  of the problem should be set to a
 *                                  negative value
 *                                  (default = [])
 * - spatial_discretization_order:  order of discretization for 
 *                                  spatial derivatives
 *                                  (default = 2)
 *
 * Return values:
 * - distance_function:             distance function
 * - extension_fields:              extension fields
 *
 * NOTES:
 * - All data arrays are assumed to be in the order generated by the
 *   MATLAB meshgrid() function.  That is, data corresponding to the
 *   point (x_i,y_j) is stored at index (j,i).
 *
 *===========================================================================*/

#include "mex.h"
#include "LSMLIB_config.h"
#include "lsm_fast_marching_method.h" 

/* Input Arguments */
#define PHI	                      (prhs[0])
#define SOURCE_FIELDS             (prhs[1])
#define DX                        (prhs[2])
#define MASK                      (prhs[3])
#define SPATIAL_DERIVATIVE_ORDER  (prhs[4])

/* Output Arguments */
#define DISTANCE_FUNCTION         (plhs[0])
#define EXTENSION_FIELDS          (plhs[1])

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  /* field data */
  LSMLIB_REAL *phi;
  LSMLIB_REAL *mask;
  LSMLIB_REAL **source_fields;
  LSMLIB_REAL *distance_function;
  LSMLIB_REAL **extension_fields;
  int num_ext_fields;
 
  /* grid data */
  const int *grid_dims = mxGetDimensions(PHI);
  double *dX = mxGetPr(DX);
  LSMLIB_REAL dX_matlab_order[2];

  /* numerical parameters */
  int spatial_discretization_order;

  /* auxilliary variables */
  int i;
  int error_code;
  mxArray* tmp_mxArray;

  /* Check for proper number of arguments */
  if (nrhs < 3) {
    mexErrMsgTxt(
      "Insufficient number of input arguments (3 required; 2 optional)");
  } else if (nrhs > 5) {
    mexErrMsgTxt("Too many input arguments (3 required; 2 optional)");
  } else if (nlhs > 2) {
    mexErrMsgTxt("Too many output arguments.");
  }

  /* Check that the inputs have the correct floating-point precision */
#ifdef LSMLIB_DOUBLE_PRECISION
    if (!mxIsDouble(PHI)) {
      mexErrMsgTxt("Incompatible precision: LSMLIB built for double-precision but phi is single-precision");
    }
#else
    if (!mxIsSingle(PHI)) {
      mexErrMsgTxt("Incompatible precision: LSMLIB built for single-precision but phi is double-precision");
    }
#endif

  /* Get mask */
  if ( (nrhs < 4) || (mxIsEmpty(MASK)) ) {
    mask = 0;  /* NULL mask ==> all points are in interior of domain */
  } else {

#ifdef LSMLIB_DOUBLE_PRECISION
    if (!mxIsDouble(MASK)) {
      mexErrMsgTxt("Incompatible precision: LSMLIB built for double-precision but mask is single-precision");
    }
#else
    if (!mxIsSingle(MASK)) {
      mexErrMsgTxt("Incompatible precision: LSMLIB built for single-precision but mask is double-precision");
    }
#endif

    mask = (LSMLIB_REAL*) mxGetPr(MASK);
  }

  /* Get spatial derivative order */
  if (nrhs < 5) {
    spatial_discretization_order = 2;
  } else {
    spatial_discretization_order = mxGetPr(SPATIAL_DERIVATIVE_ORDER)[0];
  }

  /* Assign pointers for phi and extension field data */
  phi = (LSMLIB_REAL*) mxGetPr(PHI);
  num_ext_fields = mxGetNumberOfElements(SOURCE_FIELDS);
  source_fields = (LSMLIB_REAL**) mxMalloc(num_ext_fields*sizeof(LSMLIB_REAL*));
  for (i = 0; i < num_ext_fields; i++) {
    tmp_mxArray = mxGetCell(SOURCE_FIELDS,i);

#ifdef LSMLIB_DOUBLE_PRECISION
    if (!mxIsDouble(tmp_mxArray)) {
      mexErrMsgTxt("Incompatible precision: LSMLIB built for double-precision but one of the source fields is single-precision");
    }
#else
    if (!mxIsSingle(tmp_mxArray)) {
      mexErrMsgTxt("Incompatible precision: LSMLIB built for single-precision but one of the source fields is double-precision");
    }
#endif

    source_fields[i] = (LSMLIB_REAL*) mxGetPr(tmp_mxArray);
  }


  /* Create distance function and extension field data */
#ifdef LSMLIB_DOUBLE_PRECISION
  DISTANCE_FUNCTION = mxCreateDoubleMatrix(grid_dims[0], grid_dims[1], mxREAL);
#else
  DISTANCE_FUNCTION = mxCreateNumericMatrix(grid_dims[0], grid_dims[1],
                                            mxSINGLE_CLASS, mxREAL);
#endif                                
  distance_function = (LSMLIB_REAL*) mxGetPr(DISTANCE_FUNCTION);
  num_ext_fields = mxGetNumberOfElements(SOURCE_FIELDS);
  EXTENSION_FIELDS = mxCreateCellArray(1, &num_ext_fields);
  extension_fields = 
    (LSMLIB_REAL**) mxMalloc(num_ext_fields*sizeof(LSMLIB_REAL*)); 
  for (i = 0; i < num_ext_fields; i++) {
#ifdef LSMLIB_DOUBLE_PRECISION
    tmp_mxArray = mxCreateDoubleMatrix(grid_dims[0], grid_dims[1], mxREAL);
#else
    tmp_mxArray= mxCreateNumericMatrix(grid_dims[0], grid_dims[1],
                                       mxSINGLE_CLASS, mxREAL);
#endif                                
    mxSetCell(EXTENSION_FIELDS, i, tmp_mxArray);
    extension_fields[i] = (LSMLIB_REAL*) mxGetPr(tmp_mxArray);
  }

  /* Change order of dX to be match MATLAB meshgrid() order for grids. */
  dX_matlab_order[0] = dX[1];
  dX_matlab_order[1] = dX[0];

  /* Carry out FMM calculation */
  error_code = computeExtensionFields2d(
                 distance_function,
                 extension_fields,
                 phi,
                 mask,
                 source_fields,
                 num_ext_fields,
                 spatial_discretization_order,
                 (int*) grid_dims,
                 dX_matlab_order);

  if (error_code) {
    mexErrMsgTxt("computeExtensionFields2d failed...");
  }

  /* Clean up memory */
  mxFree(source_fields); 
  mxFree(extension_fields);

  return;
}
