
/************************************************************************
*    FILE NAME:       visualcomponentquad.h
*
*    DESCRIPTION:     Class for handling the visual part of 2d quad
************************************************************************/

#ifndef __visual_component_quad_h__
#define __visual_component_quad_h__

// Physical component dependency
#include <common/ivisualcomponent.h>

// Game lib dependencies
#include <utilities/matrix.h>
#include <common/color.h>
#include <common/size.h>

// Boost lib dependencies
#include <boost/noncopyable.hpp>

// Forward declaration(s)
class CObjectVisualData2D;

class CVisualComponentQuad : public iVisualComponent, boost::noncopyable
{
public:

    // Constructor
    CVisualComponentQuad( const CObjectVisualData2D & visualData );

    // Destructor
    virtual ~CVisualComponentQuad();
    
private:
    
    // Reference to object visual data
    const CObjectVisualData2D & m_rVisualData;

};

#endif  // __visual_component_quad_h__


