/*********************                                                        */
/*! \file SmtCore.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Guy Katz, Parth Shah
 ** This file is part of the Marabou project.
 ** Copyright (c) 2017-2019 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved. See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** [[ Add lengthier description here ]]

**/

#ifndef __SmtCore_h__
#define __SmtCore_h__

#include "context/context.h"
#include "context/cdlist.h"
#include "PiecewiseLinearCaseSplit.h"
#include "PiecewiseLinearConstraint.h"
#include "Stack.h"
#include "Statistics.h"
#include "TrailEntry.h"

#define SMT_LOG( x, ... ) LOG( GlobalConfiguration::SMT_CORE_LOGGING, "SmtCore: %s\n", x )

class EngineState;
class Engine;
class String;

class SmtCore
{
public:
    SmtCore( IEngine *engine, CVC4::context::Context &context );
    ~SmtCore();

    /*
      Clear the stack.
    */
    void freeMemory();

    /*
      Inform the SMT core that a PL constraint is violated.
    */
    void reportViolatedConstraint( PiecewiseLinearConstraint *constraint );

    /*
      Get the number of times a specific PL constraint has been reported as
      violated.
    */
    unsigned getViolationCounts( PiecewiseLinearConstraint* constraint ) const;

    /*
      Reset all reported violation counts.
    */
    void resetReportedViolations();

    /*
      Returns true iff the SMT core wants to perform a case split.
    */
    bool needToSplit() const;

    /*
      Perform the split according to the constraint marked for
      splitting. Update bounds, add equations and update the stack.
    */
    void performSplit();

    /*
      Pop an old split from the stack, and perform a new split as
      needed. Return true if successful, false if the stack is empty.
    */
    bool popSplit();

    /*
      The current stack depth.
    */
    unsigned getStackDepth() const;

    /*
      Let the smt core know of an implied valid case split that was discovered.
    */
    void recordImpliedValidSplit( PiecewiseLinearCaseSplit &validSplit );

    /*
      Let the smt core trail know of an implied valid case split that was discovered.
    */
    void recordImpliedValidCaseSplit( PiecewiseLinearConstraint *constraint, unsigned phase );

    /*
      Return a list of all splits performed so far, both SMT-originating and valid ones,
      in the correct order.
    */
    void allSplitsSoFar( List<PiecewiseLinearCaseSplit> &result ) const;

    /*
      Get trail begin iterator.
    */
    CVC4::context::CDList<TrailEntry>::const_iterator trailBegin() const
    {
        return _trail.begin();
    };

    /*
      Get trail end iterator.
    */
    CVC4::context::CDList<TrailEntry>::const_iterator trailEnd() const
    {
        return _trail.end();
    };

    /*
      Have the SMT core start reporting statistics.
    */
    void setStatistics( Statistics *statistics );

    /*
      Have the SMT core choose, among a set of violated PL constraints, which
      constraint should be repaired (without splitting)
    */
    PiecewiseLinearConstraint *chooseViolatedConstraintForFixing( List<PiecewiseLinearConstraint *> &_violatedPlConstraints ) const;

    void setConstraintViolationThreshold( unsigned threshold );

    /*
      Pick the piecewise linear constraint for splitting, returns true
      if a constraint for splitting is successfully picked
    */
    bool pickSplitPLConstraint();

    /*
      For debugging purposes only - store a correct possible solution
    */
    void storeDebuggingSolution( const Map<unsigned, double> &debuggingSolution );
    bool checkSkewFromDebuggingSolution();
    bool splitAllowsStoredSolution( const PiecewiseLinearCaseSplit &split, String &error ) const;

private:
    /*
      A stack entry consists of the engine state before the split,
      the active split, the alternative splits (in case of backtrack),
      and also any implied splits that were discovered subsequently.
    */
    struct StackEntry
    {
    public:
        PiecewiseLinearCaseSplit _activeSplit;
        PiecewiseLinearConstraint *_sourceConstraint;
        List<PiecewiseLinearCaseSplit> _impliedValidSplits;
        List<PiecewiseLinearCaseSplit> _alternativeSplits;
        EngineState *_engineState;
    };



    /*
      Valid splits that were implied by level 0 of the stack.
    */
    List<PiecewiseLinearCaseSplit> _impliedValidSplitsAtRoot;

    /*
      Collect and print various statistics.
    */
    Statistics *_statistics;


    /*
      CVC4 Context, constructed in Engine
    */
    CVC4::context::Context& _context;

    /*
      The case-split stack.
    */
    List<StackEntry *> _stack;

    /*
      Trail is context dependent and contains all the asserted PWLCaseSplits. 
      TODO: Abstract from PWLCaseSplits to Literals
    */
    CVC4::context::CDList<TrailEntry> _trail;

    /*
      The engine.
    */
    IEngine *_engine;

    /*
      Do we need to perform a split and on which constraint.
    */
    bool _needToSplit;
    PiecewiseLinearConstraint *_constraintForSplitting;

    /*
      Count how many times each constraint has been violated.
    */
    Map<PiecewiseLinearConstraint *, unsigned> _constraintToViolationCount;

    /*
      For debugging purposes only
    */
    Map<unsigned, double> _debuggingSolution;

    /*
      A unique ID allocated to every state that is stored, for
      debugging purposes.
    */
    unsigned _stateId;

    /*
      Split when some relu has been violated for this many times
    */
    unsigned _constraintViolationThreshold;

    /*
      Helper function to establish equivalence between trail and stack information
     */
    bool checkStackTrailEquivalence();
};

#endif // __SmtCore_h__

//
// Local Variables:
// compile-command: "make -C ../.. "
// tags-file-name: "../../TAGS"
// c-basic-offset: 4
// End:
//
