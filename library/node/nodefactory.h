/************************************************************************
*    FILE NAME:       nodefactory.h
*
*    DESCRIPTION:     Class factory for node creation
************************************************************************/

#pragma once

// Forward declaration(s)
class iNode;
class CNodeData;

namespace NNodeFactory
{
    // Create the control
    iNode * Create( const CNodeData & rNodeData );
};
