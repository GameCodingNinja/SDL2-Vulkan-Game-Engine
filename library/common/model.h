/************************************************************************
*    FILE NAME:       model.h
*
*    DESCRIPTION:     Model class
************************************************************************/  

#pragma once

// Game lib dependencies
#include <common/mesh.h>

class CModel
{
public:

    // Loaded mesh data
    std::vector<CMesh> m_meshVec;
};
