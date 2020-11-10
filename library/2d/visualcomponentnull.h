
/************************************************************************
*    FILE NAME:       visualcomponentnull.h
*
*    DESCRIPTION:     Null visual component
************************************************************************/

#pragma once

// Physical component dependency
#include <common/ivisualcomponent.h>

// Boost lib dependencies
#include <boost/noncopyable.hpp>

// Forward declaration(s)
class CObjectData2D;

class CVisualComponentNull : public iVisualComponent, boost::noncopyable
{
public:

    // Constructor
    CVisualComponentNull( const iObjectData & objectData );

    // Destructor
    virtual ~CVisualComponentNull();
};
