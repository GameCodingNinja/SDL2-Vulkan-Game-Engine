
/************************************************************************
*    FILE NAME:       spritesheetmanager.h
*
*    DESCRIPTION:     Temporary container for loading sprite sheet data
*                     so that the same large complex xml is not reloaded.
************************************************************************/

#pragma once

// Game lib dependencies
#include <sprite/spritesheet.h>

// Standard lib dependencies
#include <string>
#include <map>

class CSpriteSheetMgr
{
public:
    
    // Get the instance of the singleton class
    static CSpriteSheetMgr & Instance()
    {
        static CSpriteSheetMgr spriteSheetMgr;
        return spriteSheetMgr;
    }
    
    // Load the glyph data from XML file
    const CSpriteSheet & load( const std::string & filePath );
    
    // Clear all the sprite sheet data
    void clear();
    
private:

    // Constructor
    CSpriteSheetMgr();

    // Destructor
    ~CSpriteSheetMgr();
    
private:
    
    // Map of sprite sheet data
    std::map< const std::string, CSpriteSheet > m_spriteSheetMap;
};
