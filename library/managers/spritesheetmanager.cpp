
/************************************************************************
*    FILE NAME:       spritesheetmanager.cpp
*
*    DESCRIPTION:     Temporary container for loading sprite sheet data
*                     so that the same large complex xml is not reloaded.
************************************************************************/

// Physical component dependency
#include <managers/spritesheetmanager.h>


/************************************************************************
*    DESC:  Constructor
************************************************************************/
CSpriteSheetMgr::CSpriteSheetMgr()
{
}


/************************************************************************
*    DESC:  destructor
************************************************************************/
CSpriteSheetMgr::~CSpriteSheetMgr()
{
}


/************************************************************************
*    DESC:  Load the glyph data from XML file
************************************************************************/
const CSpriteSheet & CSpriteSheetMgr::load( const std::string & filePath )
{
    auto mapIter = m_spriteSheetMap.find( filePath );
    if( mapIter == m_spriteSheetMap.end() )
    {
        // Add a new entry to the map
        mapIter = m_spriteSheetMap.emplace( filePath, CSpriteSheet() ).first;

        // Load the glyph data from XML file
        mapIter->second.load( filePath );
    }

    return mapIter->second;
}


/************************************************************************
*    DESC:  Clear all the sprite sheet data
************************************************************************/
void CSpriteSheetMgr::clear()
{
    m_spriteSheetMap.clear();
}
