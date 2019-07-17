
/************************************************************************
*    FILE NAME:       visualcomponentnull.h
*
*    DESCRIPTION:     Null visual component
************************************************************************/

#ifndef __visual_component_null_h__
#define __visual_component_null_h__

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

#endif
