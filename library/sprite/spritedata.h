
/************************************************************************
*    FILE NAME:       spritedata.h
*
*    DESCRIPTION:     Sprite Data Class
************************************************************************/

#pragma once

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
        const std::string & nodeName,
        const std::string & defGroup = std::string(),
        const std::string & defObjName = std::string() );

    CSpriteData( 
        const std::string & group,
        const std::string & objName );
    
    CSpriteData( const CSpriteData & data );
    
    // Destructor
    virtual ~CSpriteData();
    
    // Get the group
    const std::string & getGroup() const;
    
    // Get the object name
    const std::string & getObjectName() const;

    // Get the XML Node
    const XMLNode & getXMLNode() const;

private:

    // XML Node holding sprite data
    XMLNode m_xmlNode;
    
    // Group Name
    std::string m_group;
    
    // Object name
    std::string m_objectName;
};
