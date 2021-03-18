/*********************                                                        */
/*! \file Statistics.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Guy Katz, Andrew Wu, Duligur Ibeling
 ** This file is part of the Marabou project.
 ** Copyright (c) 2017-2019 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved. See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** [[ Add lengthier description here ]]

 **/

#ifndef __Statistics_h__
#define __Statistics_h__

#include "Map.h"
#include "TimeUtils.h"

class Statistics
{
public:
    Statistics();

    enum StatisticsUnsignedAttr
    {
     // Overall
     NUM_PIECEWISE_LINEAR_CONSTRAINTS,
     NUM_ACTIVE_PIECEWISE_LINEAR_CONSTRAINTS,

     // Preprocessing
     NUM_EQUATIONS_REMOVED_BY_PREPROCESSING,
     NUM_CONSTRAINTS_REMOVED_BY_PREPROCESSING,
     NUM_VARIABLES_REMOVED_BY_PREPROCESSING,

     // Search
     CURRENT_STACK_DEPTH,
     NUM_VISITED_TREE_STATES,
    };

    enum StatisticsLongAttr
    {
     // Overall
     NUM_MAIN_LOOP_ITERATIONS,
     TIME_MAIN_LOOP_MICRO,

     // Preprocessing
     TIME_PREPROCESSING_MICRO,

     // Search
     TIME_BRANCHING_HEURISTICS_MICRO,
     TIME_SMT_CORE_PUSH_MICRO,
     TIME_SMT_CORE_POP_MICRO,
     TIME_CHECKING_QUIT_CONDITION_MICRO,

     // Simplex
     NUM_SIMPLIEX_STEPS,
     NUM_SIMPLIEX_CALLS,
     NUM_PROPOSED_FLIPS,
     NUM_REJECTED_FLIPS,
     NUM_ACCEPTED_FLIPS,
     TIME_SIMPLEX_STEPS_MICRO,
     TIME_COMPUTE_HEURISTIC_COST_MICRO,
     TIME_UPDATING_COST_FUNCTION_MICRO,
     TIME_COLLECTING_VIOLATED_PLCONSTRAINT_MICRO,
     TIME_ADDING_CONSTRAINTS_TO_LP_SOLVER_MICRO,

     // Tightening
     NUM_COST_LEMMAS,
     NUM_EXPLICIT_BASIS_BOUND_TIGHTENING_ATTEMPT,
     NUM_CONSTRAINT_MATRIX_BOUND_TIGHTENING_ATTEMPT,
     NUM_SYMBOLIC_BOUND_TIGHTENING_ATTEMPT,
     NUM_LP_BOUND_TIGHTENING_ATTEMPT,
     NUM_EXPLICIT_BASIS_BOUND_TIGHTENING,
     NUM_CONSTRAINT_MATRIX_BOUND_TIGHTENING,
     NUM_SYMBOLIC_BOUND_TIGHTENING,
     NUM_LP_BOUND_TIGHTENING,

     TIME_EXPLICIT_BASIS_BOUND_TIGHTENING_MICRO,
     TIME_CONSTRAINT_MATRIX_TIGHTENING_MICRO,
     TIME_SYMBOLIC_BOUND_TIGHTENING_MICRO,
     TIME_LP_TIGHTENING_MICRO,
     TIME_PERFORMING_VALID_CASE_SPLITS_MICRO,

     // Statistics
     TIME_HANDLING_STATISTICS_MICRO,
    };

    enum StatisticsDoubleAttr
    {
     // Local search
     HEURISTIC_COST,

    };

    /*
      Print the current statistics.
    */
    void print();

    void resetTimeStatsForMainLoop();

    /*
      Set starting time of the main loop.
    */
    void stampStartingTime();

    inline void setUnsignedAttr( StatisticsUnsignedAttr attr, unsigned value )
    {
        _unsignedAttributes[attr] = value;
    }

    inline void setLongAttr( StatisticsLongAttr attr, unsigned long long value )
    {
        _longAttributes[attr] = value;
    }

    inline void setDoubleAttr( StatisticsDoubleAttr attr, double value )
    {
        _doubleAttributes[attr] = value;
    }

    inline void incUnsignedAttr( StatisticsUnsignedAttr attr, unsigned value )
    {
        _unsignedAttributes[attr] += value;
    }

    inline void incLongAttr( StatisticsLongAttr attr, unsigned long long value )
    {
        _longAttributes[attr] += value;
    }

    inline void incDoubleAttr( StatisticsDoubleAttr attr, double value )
    {
        _doubleAttributes[attr] += value;
    }

    inline unsigned getUnsignedAttr( StatisticsUnsignedAttr attr ) const
    {
        return _unsignedAttributes[attr];
    }

    inline unsigned long long  getLongAttr( StatisticsLongAttr attr ) const
    {
        return _longAttributes[attr];
    }

    inline double getDoubleAttr( StatisticsDoubleAttr attr ) const
    {
        return _doubleAttributes[attr];
    }

    /*
      Report a timeout, or check whether a timeout has occurred
    */
    unsigned long long getTotalTime() const;
    void timeout();
    bool hasTimedOut() const;

private:
    // Initial timestamp
    struct timespec _startTime;

    Map<StatisticsUnsignedAttr, unsigned> _unsignedAttributes;

    Map<StatisticsLongAttr, unsigned long long> _longAttributes;

    Map<StatisticsDoubleAttr, double> _doubleAttributes;

    // Whether the engine quitted with a timeout
    bool _timedOut;

    // Printing helpers
    double printPercents( unsigned long long part, unsigned long long total ) const;
    double printAverage( unsigned long long part, unsigned long long total ) const;
};

#endif // __Statistics_h__

//
// Local Variables:
// compile-command: "make -C ../.. "
// tags-file-name: "../../TAGS"
// c-basic-offset: 4
// End:
//
