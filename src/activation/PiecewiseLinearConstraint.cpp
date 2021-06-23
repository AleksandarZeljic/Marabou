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
   : _gurobi( nullptr )
   , _constraintActive( true )
   , _phaseStatus( PHASE_NOT_FIXED )
   , _numCases( 0 )
   , _boundManager( nullptr )
   , _context( nullptr )
   , _cdConstraintActive( nullptr )
   , _cdPhaseStatus( nullptr )
   , _cdInfeasibleCases( nullptr )
   , _score( FloatUtils::negativeInfinity() )
   , _statistics( NULL )
   , _phaseOfHeuristicCost( PHASE_NOT_FIXED )
{
}

PiecewiseLinearConstraint::PiecewiseLinearConstraint(
    unsigned numCases )
   : _gurobi(nullptr)
   , _constraintActive( true )
   , _phaseStatus( PHASE_NOT_FIXED )
   , _numCases( numCases )
   , _boundManager( nullptr )
   , _context( nullptr )
   , _cdConstraintActive( nullptr )
   , _cdPhaseStatus( nullptr )
   , _cdInfeasibleCases( nullptr )
   , _score( FloatUtils::negativeInfinity() )
   , _statistics( NULL )
   , _phaseOfHeuristicCost( PHASE_NOT_FIXED )
{
}

void PiecewiseLinearConstraint::setStatistics( Statistics *statistics )
{
    _statistics = statistics;
}

void PiecewiseLinearConstraint::setActiveConstraint( bool active )
{
    if ( _cdConstraintActive != nullptr )
        *_cdConstraintActive = active;
    else
        _constraintActive = active;
}

bool PiecewiseLinearConstraint::isActive() const
{
    if ( _cdConstraintActive != nullptr )
        return *_cdConstraintActive;
    else
        return _constraintActive;
}

void PiecewiseLinearConstraint::registerBoundManager(
    BoundManager *boundManager )
{
    ASSERT( _boundManager == nullptr );
    _boundManager = boundManager;
}

void PiecewiseLinearConstraint::initializeCDOs(
    CVC4::context::Context *context )
{
    ASSERT( _context == nullptr );
    _context = context;

    initializeCDActiveStatus();
    initializeCDPhaseStatus();
    initializeCDInfeasibleCases();
}

void PiecewiseLinearConstraint::initializeCDInfeasibleCases()
{
    ASSERT( _context != nullptr );
    ASSERT( _cdInfeasibleCases == nullptr );
    _cdInfeasibleCases = new ( true ) CVC4::context::CDList<PhaseStatus>( _context );
}

void PiecewiseLinearConstraint::initializeCDActiveStatus()
{
    ASSERT( _context != nullptr );
    ASSERT( _cdConstraintActive == nullptr );
    _cdConstraintActive = new ( true ) CVC4::context::CDO<bool>( _context, _constraintActive );
}

void PiecewiseLinearConstraint::initializeCDPhaseStatus()
{
    ASSERT( _context != nullptr );
    ASSERT( _cdPhaseStatus == nullptr );
    _cdPhaseStatus =
        new ( true ) CVC4::context::CDO<PhaseStatus>( _context, _phaseStatus );
}

void PiecewiseLinearConstraint::cdoCleanup()
{
    if ( _cdConstraintActive != nullptr )
        _cdConstraintActive->deleteSelf();

    _cdConstraintActive = nullptr;

    if ( _cdPhaseStatus != nullptr )
        _cdPhaseStatus->deleteSelf();

    _cdPhaseStatus = nullptr;

    if ( _cdInfeasibleCases != nullptr )
        _cdInfeasibleCases->deleteSelf();

    _cdInfeasibleCases = nullptr;

    _context = nullptr;
}

PhaseStatus PiecewiseLinearConstraint::getPhaseStatus() const
{
    if ( _cdPhaseStatus != nullptr )
        return *_cdPhaseStatus;
    else
        return _phaseStatus;
}

void PiecewiseLinearConstraint::setPhaseStatus( PhaseStatus phaseStatus )
{
    if ( _cdPhaseStatus != nullptr )
        *_cdPhaseStatus = phaseStatus;
    else
        _phaseStatus = phaseStatus;
}

void PiecewiseLinearConstraint::initializeDuplicateCDOs(
    PiecewiseLinearConstraint *clone ) const
{
    if ( clone->_context != nullptr )
    {
        ASSERT( clone->_cdConstraintActive != nullptr );
        clone->_cdConstraintActive = nullptr;
        clone->initializeCDActiveStatus();
        clone->setActiveConstraint( this->isActive() );

        ASSERT( clone->_cdPhaseStatus != nullptr );
        clone->_cdPhaseStatus = nullptr;
        clone->initializeCDPhaseStatus();
        clone->setPhaseStatus( this->getPhaseStatus() );

        ASSERT( clone->_cdInfeasibleCases != nullptr );
        clone->_cdInfeasibleCases = nullptr;
        clone->initializeCDInfeasibleCases();
        // Does not copy contents
    }
}

void PiecewiseLinearConstraint::markInfeasible(
    PhaseStatus infeasibleCase )
{
    _cdInfeasibleCases->push_back( infeasibleCase );
}

PhaseStatus PiecewiseLinearConstraint::nextFeasibleCase()
{
    ASSERT( getPhaseStatus() == PHASE_NOT_FIXED );

    if ( !isFeasible() )
        return CONSTRAINT_INFEASIBLE;

    List<PhaseStatus> allCases = getAllCases();
    for ( PhaseStatus thisCase : allCases )
    {
        auto loc =
            std::find( _cdInfeasibleCases->begin(), _cdInfeasibleCases->end(), thisCase );

        // Case is not infeasible, return it as feasible
        if ( loc == _cdInfeasibleCases->end() )
            return thisCase;
    }

    // UNREACHABLE
    ASSERT( false );
    return CONSTRAINT_INFEASIBLE;
}

bool PiecewiseLinearConstraint::isCaseInfeasible( PhaseStatus phase ) const
{
    ASSERT( _cdInfeasibleCases );
    return std::find( _cdInfeasibleCases->begin(), _cdInfeasibleCases->end(), phase ) != _cdInfeasibleCases->end();
}

//
// Local Variables:
// compile-command: "make -C ../.. "
// tags-file-name: "../../TAGS"
// c-basic-offset: 4
// End:
//
