/*********************                                                        */
/*! \file PiecewiseLinearConstraint.cpp
 ** \verbatim
 ** Top contributors (to current version):
 **   Guy Katz
 ** This file is part of the Marabou project.
 ** Copyright (c) 2017-2019 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved. See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** [[ Add lengthier description here ]]

**/

#include "GlobalConfiguration.h"
#include "PiecewiseLinearConstraint.h"
#include "Statistics.h"

PiecewiseLinearConstraint::PiecewiseLinearConstraint()
    : _boundManager( NULL )
    , _gurobi( NULL )
    , _context( NULL )
    , _constraintActive( NULL )
    , _phaseStatus( NULL )
    , _score( FloatUtils::negativeInfinity() )
    , _statistics( NULL )
    , _phaseOfHeuristicCost( PHASE_NOT_FIXED )
{
}

void PiecewiseLinearConstraint::setStatistics( Statistics *statistics )
{
    _statistics = statistics;
}

//
// Local Variables:
// compile-command: "make -C ../.. "
// tags-file-name: "../../TAGS"
// c-basic-offset: 4
// End:
//
