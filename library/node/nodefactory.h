/************************************************************************
*    FILE NAME:       nodefactory.h
*
*    DESCRIPTION:     Class factory for node creation
************************************************************************/

#ifndef __node_factory_h__
#define __node_factory_h__

// Game lib dependencies
#include <common/point.h>
#include <common/defs.h>
#include <common/worldvalue.h>

// Forward declaration(s)
class iNode;
class CNodeData;
class CNodeDataList;

namespace NNodeFactory
{
    // Create the control
    iNode * Create(
    const CNodeData & rNodeData,
    const int nodeId,
    const CPoint<CWorldValue> & pos = CPoint<CWorldValue>(),
    const CPoint<float> & rot = CPoint<float>(),
    const CPoint<float> & scale = CPoint<float>(1,1,1) );
};

#endif
