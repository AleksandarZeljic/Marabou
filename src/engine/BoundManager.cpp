/*********************                                                        */
/*! \file BoundManager.cpp
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

#include "FloatUtils.h"
#include "BoundManager.h"
#include "Debug.h"
#include "Tableau.h"
#include "Tightening.h"

using namespace CVC4::context;

BoundManager::BoundManager( Context &context )
    : _context( context )
    , _size( 0 )
{
};

BoundManager::~BoundManager()
{
    for ( unsigned i = 0; i < _size; ++i)
    {
        _lowerBounds[i]->deleteSelf();
        _upperBounds[i]->deleteSelf();
        _tightenedLower[i]->deleteSelf();
        _tightenedUpper[i]->deleteSelf();
    }
};

unsigned BoundManager::registerNewVariable()
{
    ASSERT( _lowerBounds.size() == _size );
    ASSERT( _upperBounds.size() == _size );
    ASSERT( _tightenedLower.size() == _size );
    ASSERT( _tightenedUpper.size() == _size );

    unsigned newVar = _size++;

    _lowerBounds.append( new (true) CDO<double>( &_context ) );
    _upperBounds.append( new (true) CDO<double>( &_context ) );
    _tightenedLower.append( new (true) CDO<bool>( &_context ) );
    _tightenedUpper.append( new (true) CDO<bool>( &_context ) );

    *_lowerBounds[newVar] = FloatUtils::negativeInfinity();
    *_upperBounds[newVar] = FloatUtils::infinity();

    *_tightenedLower[newVar] = false;
    *_tightenedUpper[newVar] = false;

    ASSERT( _lowerBounds.size() == _size );
    ASSERT( _upperBounds.size() == _size );
    ASSERT( _tightenedLower.size() == _size );
    ASSERT( _tightenedUpper.size() == _size );

    return newVar;
}

void BoundManager::initialize( unsigned numberOfVariables )
{
    ASSERT( 0 == _size);

    for ( unsigned i = 0; i < numberOfVariables; ++i)
        registerNewVariable();

    ASSERT( numberOfVariables == _size );
}

bool BoundManager::tightenLowerBound( unsigned variable, double value )
{
    bool tightened = setLowerBound( variable, value );
    if ( tightened &&  nullptr != _tableau )
        _tableau->postLowerBoundUpdate( variable, value );
    return tightened;
}

bool BoundManager::tightenUpperBound( unsigned variable, double value )
{
    bool tightened = setUpperBound( variable, value );
    if ( tightened && nullptr != _tableau )
        _tableau->postUpperBoundUpdate( variable, value );
    return tightened;
}
bool BoundManager::setLowerBound( unsigned variable, double value )
{
    ASSERT( variable < _size );
    if ( value > getLowerBound( variable ) )
    {
        *_lowerBounds[variable] = value;
        *_tightenedLower[variable] = true;
        return true;
    }
    return false;
}

bool BoundManager::setUpperBound( unsigned variable, double value )
{
    ASSERT( variable < _size );
    if ( value < getUpperBound( variable ) )
    {
        *_upperBounds[variable] = value ;
        *_tightenedUpper[variable] = true;
        return true;
    }
    return false;
}

double BoundManager::getLowerBound( unsigned variable )
{
  ASSERT( variable < _size );
  return *_lowerBounds[variable];
}


double BoundManager::getUpperBound( unsigned variable )
{
  ASSERT( variable < _size );
  return *_upperBounds[variable];
}

void BoundManager::getTightenings( List<Tightening> &tightenings ) 
{
    for ( unsigned i = 0; i < _size; ++i )
    {
        if ( *_tightenedLower[i] )
        {
            tightenings.append( Tightening( i, *_lowerBounds[i], Tightening::LB ) );
            *_tightenedLower[i] = false;
        }

        if ( *_tightenedUpper[i] )
        {
            tightenings.append( Tightening( i, *_upperBounds[i], Tightening::UB ) );
            *_tightenedUpper[i] = false;
        }
    }
}


void BoundManager::registerTableauReference( Tableau *ptrTableau )
{
    _tableau = ptrTableau;
}

unsigned BoundManager::getSize( )
{
  return _size;
}

//
// Local Variables:
// compile-command: "make -C ../.. "
// tags-file-name: "../../TAGS"
// c-basic-offset: 4
// End:
//
