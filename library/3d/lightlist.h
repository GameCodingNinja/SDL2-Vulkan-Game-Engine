
/************************************************************************
*    FILE NAME:       lightlist.h
*
*    DESCRIPTION:     Light List Class
************************************************************************/

#pragma once

// Game lib dependencies
#include <common/defs.h>
#include <common/color.h>
#include <common/point.h>

// Standard lib dependencies
#include <string>
#include <map>

class CLightLst
{
public:

    // Constructor
    CLightLst();

    // Destructor
    virtual ~CLightLst();
    
    // Add a light
    void add(
        const std::string & name,
        ELightType lightType,
        const CColor & color,
        bool enable = false,
        const CPoint<float> & posDir = CPoint<float>(0,0,0),
        float radius = 0.f );
    
    // Turn the light on or off
    void enable( const std::string & name, bool enable = true );
    
private:
    
    std::map<std::string, class CLight> m_lightMap;

};
