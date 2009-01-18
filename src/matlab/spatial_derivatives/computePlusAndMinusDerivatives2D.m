%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
% computePlusAndMinusDerivatives2D() computes the upwind Hamilton-Jacobi
% spatial derivatives of the specified order for 2D grid functions.
%
% Usage: [phi_x_plus, phi_y_plus, ...
%         phi_x_minus, phi_y_minus] = ...
%        computePlusAndMinusDerivatives2D(phi, ...
%                                         ghostcell_width, ...
%                                         dX, ...
%                                         spatial_derivative_order)
%
% Arguments:
% - phi:                       function for which to compute upwind 
%                                derivative
% - ghostcell_width:           number of ghostcells at boundary of
%                                computational domain
% - dX:                        array containing the grid spacing
%                                in coordinate directions
% - spatial_derivative_order:  spatial derivative order
%                                (default = 1)
%
% Return values:
% - phi_x_plus:                x-component of plus HJ ENO/WENO derivative
% - phi_y_plus:                y-component of plus HJ ENO/WENO derivative
% - phi_x_minus:               x-component of minus HJ ENO/WENO derivative
% - phi_y_minus:               y-component of minus HJ ENO/WENO derivative
%
% NOTES:
% - phi_x_plus, phi_y_plus, phi_x_minus and phi_y_minus have the same 
%   ghostcell width as phi.
%
% - All data arrays are assumed to be in the order generated by the
%   MATLAB meshgrid() function.  That is, data corresponding to the
%   point (x_i,y_j) is stored at index (j,i).  The output data arrays
%   will be returned with the same ordering as the input data arrays.
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
% Copyright:  (c) 2005-2008 Kevin T. Chu and Masa Prodanovic
% Revision:   $Revision: 1.1 $
% Modified:   $Date$
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function [phi_x_plus, phi_y_plus, ...
          phi_x_minus, phi_y_minus] = ...
         computePlusAndMinusDerivatives2D(phi, ...
                                          ghostcell_width, ...
                                          dX, ...
                                          spatial_derivative_order)

% parameter checks
if (nargin < 4)
  error('MATLAB:missingArgs','computePlusAndMinusDerivatives2D:missing arguments');
end

if (nargin < 5)
  spatial_derivative_order = 1;
else
  if ( (spatial_derivative_order ~= 1) & (spatial_derivative_order ~= 2) ...
     & (spatial_derivative_order ~= 3) & (spatial_derivative_order ~= 5) )

    error('computePlusAndMinusDerivatives2D:Invalid spatial derivative order...only 1, 2, 3, and 5 are supported');
  end
end

switch (spatial_derivative_order)

  case 1
    [phi_x_plus, phi_y_plus, phi_x_minus, phi_y_minus] = ...
    HJ_ENO1_2D(phi, ghostcell_width, dX);

  case 2
    [phi_x_plus, phi_y_plus, phi_x_minus, phi_y_minus] = ...
    HJ_ENO2_2D(phi, ghostcell_width, dX);

  case 3
    [phi_x_plus, phi_y_plus, phi_x_minus, phi_y_minus] = ...
    HJ_ENO3_2D(phi, ghostcell_width, dX);

  case 5
    [phi_x_plus, phi_y_plus, phi_x_minus, phi_y_minus] = ...
    HJ_WENO5_2D(phi, ghostcell_width, dX);

end
