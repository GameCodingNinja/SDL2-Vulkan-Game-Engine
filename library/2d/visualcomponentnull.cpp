
/************************************************************************
*    FILE NAME:       visualcomponentnull.cpp
*
*    DESCRIPTION:     Null visual component
************************************************************************/

// Physical component dependency
#include <2d/visualcomponentnull.h>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CVisualComponentNull::CVisualComponentNull( const iObjectData & objectData ) :
    iVisualComponent( objectData )
{
}

/************************************************************************
*    desc:  destructor                                                             
************************************************************************/
CVisualComponentNull::~CVisualComponentNull()
{
}
