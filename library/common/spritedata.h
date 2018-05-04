
/************************************************************************
*    FILE NAME:       spritedata.h
*
*    DESCRIPTION:     Sprite Data Class
************************************************************************/

#ifndef __sprite_data_h__
#define __sprite_data_h__

// Physical component dependency
#include <common/object.h>
#include <common/color.h>

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
    ~CSpriteData();
    
    // Init the script functions and add them to the map
    void loadScriptFunctions( const XMLNode & node );
    
    // Get the script functions
    const std::map<std::string, std::string> & getScriptFunctions() const;
    
    // Get the sprite name
    const std::string & getName() const;
    
    // Get the group
    const std::string & getGroup() const;
    
    // Get the object name
    const std::string & getObjectName() const;

    // Get the AI name
    const std::string & getAIName() const;
    
    // Get the unique id number
    int getId() const;
    
    // Get the font data
    const CFontData * getFontData() const;

private:

    std::string m_name;
    std::string m_group;
    std::string m_objectName;
    std::string m_aiName;
    std::map<std::string, std::string> m_scriptFunctionMap;
    int m_id;
    std::unique_ptr<CFontData> m_upFontData;
};

#endif  // __sprite_data_h__


