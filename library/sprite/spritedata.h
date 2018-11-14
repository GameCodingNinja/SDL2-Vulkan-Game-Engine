
/************************************************************************
*    FILE NAME:       spritedata.h
*
*    DESCRIPTION:     Sprite Data Class
************************************************************************/

#ifndef __sprite_data_h__
#define __sprite_data_h__

// Physical component dependency
#include <common/object.h>

// Game lib dependencies
#include <common/color.h>
#include <common/defs.h>

// Standard lib dependencies
#include <string>
#include <map>
#include <memory>

// Forward declaration(s)
struct XMLNode;
class CFontData;

class CSpriteData : public CObject
{
public:

    // Constructor
    CSpriteData(
        const XMLNode & node,
        const std::string & defGroup = std::string(),
        const std::string & defObjName = std::string(),
        const std::string & defAIName = std::string(),
        int defId = defs_SPRITE_DEFAULT_ID );
    
    CSpriteData( const CSpriteData & data );
    
    // Destructor
    virtual ~CSpriteData();
    
    // Init the script functions and add them to the map
    void loadScriptFunctions( const XMLNode & node );
    
    // Get the script functions
    const std::map<std::string, std::tuple<std::string, std::string>> & getScriptFunctions() const;
    
    // Get the sprite name
    const std::string & getSpriteName() const;
    
    // Get the group
    const std::string & getGroup() const;
    
    // Get the object name
    const std::string & getObjectName() const;

    // Get the AI name
    const std::string & getAIName() const;
    
    // Get the font data
    const CFontData * getFontData() const;
    
    // Get the unique id number
    int getSpriteId() const;
    
    // Get the sprite type
    NDefs::ESpriteType getSpriteType() const;

private:

    // Sprite name
    std::string m_name;
    
    // Group Name
    std::string m_group;
    
    // Object name
    std::string m_objectName;
    
    // AI name
    std::string m_aiName;
    
    // Script function map
    std::map<std::string, std::tuple<std::string, std::string>> m_scriptFunctionMap;
    
    // Font data
    std::shared_ptr<CFontData> m_upFontData;
    
    // Sprite Id
    int m_spriteId;
    
    // Sprite type
    NDefs::ESpriteType m_spriteType;
};

#endif


