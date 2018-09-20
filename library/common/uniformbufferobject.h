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
#include <common/rect.h>

// Standard lib dependencies
#include <string>

namespace NUBO
{
    class model_viewProj_color_additive
    {
    public:

        CMatrix model;
        CMatrix viewProj;
        CColor color;
        CColor additive;
    };
    
    class model_viewProj_color_additive_glyph
    {
    public:

        CMatrix model;
        CMatrix viewProj;
        CColor color;
        CColor additive;
        CRect <float> glyph;
    };
    
    // Get the UBO size
    int GetUboSize( const std::string & ubo );
}

#endif  // __uniform_buffer_object_h__
