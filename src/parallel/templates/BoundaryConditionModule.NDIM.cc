/*
 * File:        BoundaryConditionModule.NDIM.cc
 * Copyright:   (c) 2005-2008 Kevin T. Chu and Masa Prodanovic
 * Revision:    $Revision: 1.1 $
 * Modified:    $Date$
 * Description: Explicit template instantiation of LSMLIB classes 
 */

#include "SAMRAI_config.h"

#include "BoundaryConditionModule.h"
#include "BoundaryConditionModule.cc"

template class LSMLIB::BoundaryConditionModule<NDIM>;
