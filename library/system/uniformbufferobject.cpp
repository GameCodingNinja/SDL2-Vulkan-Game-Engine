
/************************************************************************
*    FILE NAME:       uniformbufferobject.cpp
*
*    DESCRIPTION:     Uniform Buffer Object
************************************************************************/

// Physical component dependency
#include <system/uniformbufferobject.h>

// Game lib dependencies
#include <utilities/exceptionhandling.h>

// Boost lib dependencies
#include <boost/format.hpp>

namespace NUBO
{
    /************************************************************************
    *    DESC:  Get the UBO size
    ************************************************************************/ 
    int GetUboSize( const std::string & ubo )
    {
        if( ubo == "model_viewProj_color_additive" )
            return sizeof(model_viewProj_color_additive);
        
        else if( ubo == "model_viewProj_color_additive_glyph" )
            return sizeof(model_viewProj_color_additive_glyph);
        
        else if( ubo == "model_rotate_viewProj_color_additive" )
            return sizeof(model_rotate_viewProj_color_additive);

        else
            throw NExcept::CCriticalException(
                "Vulkan Error!", boost::str( boost::format("Ubo size not defined! %s") % ubo ) );

        return 0;
    }
}
