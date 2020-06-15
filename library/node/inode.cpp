
/************************************************************************
*    FILE NAME:       inode.cpp
*
*    DESCRIPTION:     Node interface class. Also used as a leaf node
************************************************************************/

// Physical component dependency
#include <node/inode.h>

std::atomic<handle16_t> iNode::m_hAtomicIter = defs_DEFAULT_HANDLE;

/************************************************************************
*    DESC:  get the child node
*           NOTE: This is a recursive function
************************************************************************/
iNode * iNode::findChild( const uint16_t crcValue )
{
    if( crcValue == m_crcUserId )
        return this;

    return nullptr;
}