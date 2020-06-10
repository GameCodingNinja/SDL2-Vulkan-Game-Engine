/************************************************************************
*    FILE NAME:       nodefactory.h
*
*    DESCRIPTION:     Class factory for node creation
************************************************************************/

#ifndef __node_factory_h__
#define __node_factory_h__

// Forward declaration(s)
class iNode;
class CNodeData;

namespace NNodeFactory
{
    // Create the control
    iNode * Create( const CNodeData & rNodeData );
};

#endif
