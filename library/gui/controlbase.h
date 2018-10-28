
/************************************************************************
*    FILE NAME:       controlbase.h
*
*    DESCRIPTION:     Control base class
************************************************************************/

#ifndef __ui_control_base_h__
#define __ui_control_base_h__

// Physical component dependency
#include <2d/object2d.h>

// Game lib dependencies
#include <gui/uicontroldefs.h>
#include <common/dynamicoffset.h>

// Boost lib dependencies
#include <boost/noncopyable.hpp>

// Standard lib dependencies
#include <string>
#include <vector>

// Forward declaration(s)
struct XMLNode;

class CControlBase : public CObject2D, boost::noncopyable
{
public:

    // Constructor
    CControlBase( const std::string & group );

    // Destructor
    virtual ~CControlBase();
    
    // Load the initial info from XML node
    virtual void loadFromNode( const XMLNode & node );
    
    // Get the object group name
    const std::string & getGroup() const;
    
    // Get/Set the control name
    const std::string & getName() const;
    
    // Get the control type
    NUIControl::EControlType getType() const;
    
    // Load the control specific info from XML node
    virtual void loadControlFromNode( const XMLNode & node ) = 0;
    
    // Set the dynamic position
    void setDynamicPos();
    
    // Get the faction name
    const std::string & getFaction() const;
    
    // Set the alpha value of this menu
    virtual void setAlpha( float alpha ) = 0;
    float getAlpha() const;
    
private:
    
    // Load the dynamic offset data from node
    void loadDynamicOffsetFromNode( const XMLNode & node );
    
    // Load a sprite from an XML node
    virtual void loadSpriteFromNode( const XMLNode & node, size_t & fontSpriteCount ) = 0;
    
protected:
        
    // Object data group name
    std::string m_group;
    
    // Unique string id
    std::string m_name;
    
    // The type of control
    NUIControl::EControlType m_type;
    
    // Control string list
    std::vector<std::string> m_stringVec;
    
    // A name that is applied to similar controls.
    // Provides a way to check for many controls without having to use unique names
    std::string m_faction;
    
    // Dynamic offset
    CDynamicOffset m_dynamicOffset;
    
    // menu alpha value
    float m_alpha;
};

#endif
