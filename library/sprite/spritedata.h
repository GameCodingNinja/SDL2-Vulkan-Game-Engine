
/************************************************************************
*    FILE NAME:       spritedata.h
*
*    DESCRIPTION:     Sprite Data Class
************************************************************************/

#ifndef __sprite_data_h__
#define __sprite_data_h__

// Game lib dependencies
#include <common/defs.h>
#include <utilities/xmlParser.h>

// Standard lib dependencies
#include <string>

class CSpriteData
{
public:

    // Constructor
    CSpriteData(
        const XMLNode & node,
        const std::string & defGroup = std::string(),
        const std::string & defObjName = std::string(),
        int defId = defs_DEFAULT_ID );

    CSpriteData( 
        const std::string & group,
        const std::string & objName );
    
    CSpriteData( const CSpriteData & data );
    
    // Destructor
    virtual ~CSpriteData();
    
    // Get the sprite name
    const std::string & getSpriteName() const;
    
    // Get the group
    const std::string & getGroup() const;
    
    // Get the object name
    const std::string & getObjectName() const;
    
    // Get the unique id number
    int getSpriteId() const;

    // Get the XML Node
    const XMLNode & getXMLNode() const;

private:

    // XML Node holding sprite data
    XMLNode m_xmlNode;

    // Sprite name
    std::string m_name;
    
    // Group Name
    std::string m_group;
    
    // Object name
    std::string m_objectName;
    
    // Sprite Id
    int m_spriteId;
};

#endif
