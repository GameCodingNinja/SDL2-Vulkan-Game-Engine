/************************************************************************
*    FILE NAME:       uniformbufferobject.h
*
*    DESCRIPTION:     Uniform Buffer Object
************************************************************************/  

#ifndef __uniform_buffer_object_h__
#define __uniform_buffer_object_h__

// Game lib dependencies
#include <utilities/matrix.h>
#include <common/color.h>

class UniformBufferObject
{
public:
    
    CMatrix model;
    CMatrix viewProj;
    CColor color;
    CColor additive;
    int renderType = 0;
};

#endif  // __uniform_buffer_object_h__
