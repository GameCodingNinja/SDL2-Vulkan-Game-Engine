
/************************************************************************
*    FILE NAME:       inode.cpp
************************************************************************/

// Physical component dependency
#include <node/inode.h>

std::atomic<handle16_t> iNode::m_hAtomicIter = defs_DEFAULT_HANDLE;