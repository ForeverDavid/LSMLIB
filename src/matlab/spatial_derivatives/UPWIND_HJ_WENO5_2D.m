%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
% UPWIND_HJ_WENO5_2D() computes the fifth-order upwind HJ WENO
% approximation to grad(phi)
%
% Usage: [phi_x, phi_y] = ...
%        UPWIND_HJ_WENO5_2D(phi, vel_x, vel_y, ghostcell_width, dX)
%
% Arguments:
% - phi:              function for which to compute upwind
%                       derivative
% - vel_x:            x-component of velocity to use in upwinding
% - vel_y:            y-component of velocity to use in upwinding
% - ghostcell_width:  number of ghostcells at boundary of
%                       computational domain
% - dX:               array containing the grid spacing
%                       in coordinate directions
%
% Return values:
% - phi_x:            x-component of fifth-order, upwind 
%                       HJ WENO derivative
% - phi_y:            y-component of fifth-order, upwind 
%                       HJ WENO derivative
%
% NOTES:
% - The vel_x and vel_y arrays are assumed to be the same size.
%
% - phi_x and phi_y have the same ghostcell width as phi.
%
% - All data arrays are assumed to be in the order generated by the
%   MATLAB meshgrid() function.  That is, data corresponding to the
%   point (x_i,y_j) is stored at index (j,i).  The output data arrays
%   will be returned with the same ordering as the input data arrays.
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
% Copyright:  (c) 2005-2008 Kevin T. Chu and Masa Prodanovic
% Revision:   $Revision: 1.7 $
% Modified:   $Date$
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
