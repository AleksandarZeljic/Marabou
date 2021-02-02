/*********************                                                        */
/*! \file BoundManager.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Haoze Wu, Aleksandar Zeljic
 ** This file is part of the Marabou project.
 ** Copyright (c) 2017-2019 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved. See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** [[ Add lengthier description here ]]

**/

#ifndef __BoundManager_h__
#define __BoundManager_h__

#include "context/cdo.h"
#include "context/context.h"
#include "List.h"
#include "ITableau.h"
#include "Vector.h"

class Tightening;
class BoundManager
{
public:
    BoundManager( CVC4::context::Context &ctx );
    ~BoundManager();


    /*
     * Registers a new variable, grows the BoundManager size and bound vectors,
     * initializes bounds to +/-inf, and returns the new index as the new variable.
     */
    unsigned registerNewVariable( );

    void initialize( unsigned numberOfVariables );

    bool tightenLowerBound( unsigned variable, double value );
    bool tightenUpperBound( unsigned variable, double value );

    bool setLowerBound( unsigned variable, double value );
    bool setUpperBound( unsigned variable, double value );

    /*
      Returns true if the bounds for the variable is valid
    */
    bool boundValid( unsigned variable );

    inline double getLowerBound( unsigned variable )
    {
        ASSERT( variable < _size );
        return *_lowerBounds[variable];
    }

    inline double getUpperBound( unsigned variable )
    {
        ASSERT( variable < _size );
        return *_upperBounds[variable];
    }

    unsigned getSize(); //TODO: Rename to getNumberOfVariables

    void registerTableauReference( ITableau *tableau );

private:

    CVC4::context::Context &_context;
    ITableau *_tableau = nullptr;
    unsigned _size; // TODO: Make context sensitive, to account for growing
    // For now, assume variable number is the vector index
    Vector<CVC4::context::CDO<double> *> _lowerBounds;
    Vector<CVC4::context::CDO<double> *> _upperBounds;
};


#endif // __BoundManager_h__

//
// Local Variables:
// compile-command: "make -C ../.. "
// tags-file-name: "../../TAGS"
// c-basic-offset: 4
// End:
//
