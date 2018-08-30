
/************************************************************************
*    FILE NAME:       visualcomponentquad.cpp
*
*    DESCRIPTION:     Class for handling the visual part of 2d quad
************************************************************************/

// Physical component dependency
#include <2d/visualcomponentquad.h>

// Game lib dependencies
#include <objectdata/objectvisualdata2d.h>
#include <common/quad2d.h>
#include <system/device.h>

// Boost lib dependencies

// Standard lib dependencies


/************************************************************************
*    desc:  Constructor
************************************************************************/
CVisualComponentQuad::CVisualComponentQuad( const CObjectVisualData2D & visualData ) :
    iVisualComponent( visualData ),
    m_rVisualData( visualData )
{
}   // constructor


/************************************************************************
*    desc:  destructor                                                             
************************************************************************/
CVisualComponentQuad::~CVisualComponentQuad()
{
}   // destructor




