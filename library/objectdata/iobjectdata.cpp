
/************************************************************************
*    FILE NAME:       iobjectdata.h
*
*    DESCRIPTION:     Object Data interface class
************************************************************************/

// Physical component dependency
#include <objectdata/iobjectdata.h>

// Game lib dependencies
#include <objectdata/iobjectvisualdata.h>
#include <objectdata/iobjectphysicsdata.h>

CSize<int> iObjectData::m_null_size;
std::string iObjectData::m_null_string;
iObjectVisualData iObjectData::m_null_objectVisualData;
iObjectPhysicsData iObjectData::m_null_objectPhysicsData;
