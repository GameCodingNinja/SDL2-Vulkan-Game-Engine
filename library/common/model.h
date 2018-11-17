/************************************************************************
*    FILE NAME:       model.h
*
*    DESCRIPTION:     Model class
************************************************************************/  

#ifndef __model_h__
#define __model_h__

// Game lib dependencies
#include <common/mesh.h>

class CModel
{
public:

    // Loaded mesh data
    std::vector<CMesh> m_meshVec;
};

#endif
