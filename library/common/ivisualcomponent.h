
/************************************************************************
*    FILE NAME:       ivisualcomponent.h
*
*    DESCRIPTION:     iVisualComponent Class
************************************************************************/

#ifndef __i_visual_component_h__
#define __i_visual_component_h__

// Game lib dependencies
#include <common/size.h>

// Standard lib dependencies
#include <string>

// Forward declaration(s)
class CColor;

class iVisualComponent
{
public:

    // Set/Get the color
    virtual void setColor( const CColor & color ) = 0;
    virtual void setColor( float r, float g, float b, float a ) = 0;
    virtual const CColor & getColor() const = 0;
    virtual void setDefaultColor() = 0;
    virtual const CColor & getDefaultColor() const = 0;

    // Set/Get the alpha
    virtual void setAlpha( float alpha, bool allowToExceed = false ) = 0;
    virtual float getAlpha() const = 0;
    virtual void setDefaultAlpha() = 0;
    virtual float getDefaultAlpha() const = 0;

    // Set the string to display
    virtual void createFontString(){}
    virtual void createFontString( const std::string & fontString ){}

    // Get the displayed font string
    virtual const std::string & getFontString() { return m_stringDummy; };
    virtual void setFontString( const std::string & fontString ){}
    
    // Get the font size
    virtual const CSize<float> & getFontSize() const { return m_sizeDummy; };
    
    // Is this a font sprite
    virtual bool isFontSprite() { return false; };
    
private:
    
    static CSize<float> m_sizeDummy;
    static std::string m_stringDummy;
};

#endif
