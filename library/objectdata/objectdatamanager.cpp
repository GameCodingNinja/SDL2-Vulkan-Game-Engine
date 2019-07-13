/************************************************************************
 *    FILE NAME:       objectdatamanager.cpp
 *
 *    DESCRIPTION:    Singlton that holds a map of all 2D/3D object data used for
 *					 later loading
 ************************************************************************/

// Physical component dependency
#include <objectdata/objectdatamanager.h>

// Game lib dependencies
#include <common/pipeline.h>
#include <sprite/spritedata.h>
#include <utilities/exceptionhandling.h>
#include <utilities/xmlParser.h>
#include <utilities/settings.h>
#include <objectdata/objectdata2d.h>
#include <objectdata/objectdata3d.h>
#include <managers/spritesheetmanager.h>
#include <system/device.h>

// Standard lib dependencies
#include <set>

// Boost lib dependencies
#include <boost/format.hpp>

/************************************************************************
 *    DESC:  Constructor
 ************************************************************************/
CObjectDataMgr::CObjectDataMgr()
{
}


/************************************************************************
 *    DESC:  Destructor
 ************************************************************************/
CObjectDataMgr::~CObjectDataMgr()
{
}


/************************************************************************
 *    DESC:  Load all of the meshes and materials of a specific data group
 ************************************************************************/
void CObjectDataMgr::loadGroup2D( const std::string & group )
{
    // Check for a hardware extension
    std::string ext;
    if( !m_mobileExt.empty() && CSettings::Instance().isMobileDevice() )
        if( m_listTableMap.find( group + m_mobileExt ) != m_listTableMap.end() )
            ext = m_mobileExt;

    // Make sure the group we are looking has been defined in the list table file
    auto listTableIter = m_listTableMap.find( group + ext );
    if( listTableIter == m_listTableMap.end() )
        throw NExcept::CCriticalException("Obj Data List 2D Load Group Data Error!",
            boost::str( boost::format("Object data list group name can't be found (%s).\n\n%s\nLine: %s")
                % group % __FUNCTION__ % __LINE__ ));

    // Load the group data if it doesn't already exist
    if( m_objectData2DMapMap.find( group ) == m_objectData2DMapMap.end() )
    {
        // Create a new group map inside of our map
        m_objectData2DMapMap.emplace( group, std::map<const std::string, CObjectData2D>() );

        for( auto & iter : listTableIter->second )
            load2D( group, iter );
    }
    else
    {
        throw NExcept::CCriticalException("Obj Data List 2D group load Error!",
            boost::str( boost::format("Object data list group has already been loaded (%s).\n\n%s\nLine: %s")
                % group % __FUNCTION__ % __LINE__ ));
    }

    // Create the assets from data
    createFromData2D( group );

    // Free the sprite sheet data because it's no longer needed
    CSpriteSheetMgr::Instance().clear();
}


/************************************************************************
 *    DESC:  Load all object information
 ************************************************************************/
void CObjectDataMgr::load2D( const std::string & group, const std::string & filePath )
{
    // Open and parse the XML file:
    const XMLNode mainNode = XMLNode::openFileHelper( filePath.c_str(), "objectDataList2D" );

    // Get an iterator to the group
    auto groupMapIter = m_objectData2DMapMap.find( group );


    //////////////////////////////////////////////
    // Load the default data
    //////////////////////////////////////////////

    // Get the node to the default object data
    const XMLNode defaultNode = mainNode.getChildNode( "default" );

    // Class to hold a data list's default data.
    CObjectData2D defaultData;

    // If there's no default node then we just use the defaults set in the data classes
    if( !defaultNode.isEmpty() )
        defaultData.loadFromNode( defaultNode, "", "" );


    //////////////////////////////////////////////
    // Load the object data
    //////////////////////////////////////////////

    XMLNode objectListNode = mainNode.getChildNode( "objectList" );

    for( int i = 0; i < objectListNode.nChildNode(); ++i )
    {
        // Get the object data node
        const XMLNode objectNode = objectListNode.getChildNode(i);

        // Get the object's name
        const std::string name = objectNode.getAttribute( "name" );

        // Allocate the object data to the map
        auto iter = groupMapIter->second.emplace( name, defaultData );

        // Check for duplicate names
        if( !iter.second )
        {
            throw NExcept::CCriticalException("Object Data Load Group Error!",
                boost::str( boost::format("Duplicate object name (%s - %s).\n\n%s\nLine: %s")
                    % name % group % __FUNCTION__ % __LINE__ ));
        }

        // Load in the object data
        iter.first->second.loadFromNode( objectNode, group, name );
    }
}


/************************************************************************
 *    DESC:  Create the group's VBO, IBO, textures, etc
 ************************************************************************/
void CObjectDataMgr::createFromData2D( const std::string & group )
{
    // Create it from the data
    auto groupMapIter = m_objectData2DMapMap.find( group );
    if( groupMapIter != m_objectData2DMapMap.end() )
    {
        for( auto & iter : groupMapIter->second )
            iter.second.createFromData( group );
    }
    else
    {
        throw NExcept::CCriticalException("Object Create From Data Group Error!",
            boost::str( boost::format("Object data list group name can't be found (%s).\n\n%s\nLine: %s")
                % group % __FUNCTION__ % __LINE__ ));
    }
}


/************************************************************************
 *    DESC:  Free all of the meshes materials and data of a specific group
 ************************************************************************/
void CObjectDataMgr::freeGroup2D( const std::string & group )
{
    // Free the data of a specific group
    freeDataGroup2D( group );
    
    // Delete the group assets
    CDevice::Instance().deleteGroupAssets( group );
}


/************************************************************************
 *    DESC:  Free only the data of a specific group
 ************************************************************************/
void CObjectDataMgr::freeDataGroup2D( const std::string & group )
{
    // Make sure the group we are looking for exists
    if( m_listTableMap.find( group ) == m_listTableMap.end() )
        throw NExcept::CCriticalException("Obj Data List 2D Free Group Data Error!",
            boost::str( boost::format("Object data list group name can't be found (%s).\n\n%s\nLine: %s")
                % group % __FUNCTION__ % __LINE__ ));

    // Unload the group data
    auto mapIter = m_objectData2DMapMap.find( group );
    if( mapIter != m_objectData2DMapMap.end() )
        m_objectData2DMapMap.erase( mapIter );
}


/************************************************************************
 *    DESC:  Is this group and name part of 2d data?
 ************************************************************************/
bool CObjectDataMgr::isData2D( const std::string & group, const std::string & name ) const
{
    auto mapIter = m_objectData2DMapMap.find( group );
    if( mapIter != m_objectData2DMapMap.end() )
    {
        auto iter = mapIter->second.find( name );
        if( iter != mapIter->second.end() )
            return true;
    }

    return false;
}


/************************************************************************
 *    DESC:  Get a specific 2D object's data
 ************************************************************************/
const CObjectData2D & CObjectDataMgr::getData2D( const std::string & group, const std::string & name ) const
{
    auto mapIter = m_objectData2DMapMap.find( group );
    if( mapIter == m_objectData2DMapMap.end() )
        throw NExcept::CCriticalException("Obj Data List 2D Get Data Error!",
            boost::str( boost::format("Object data list group can't be found (%s).\n\n%s\nLine: %s")
                % group % __FUNCTION__ % __LINE__ ));

    auto iter = mapIter->second.find( name );
    if( iter == mapIter->second.end() )
        throw NExcept::CCriticalException("Obj Data List 2D Get Data Error!",
            boost::str( boost::format("Object data list name can't be found (%s).\n\n%s\nLine: (%s - %s)")
                % group % name % __FUNCTION__ % __LINE__ ));

    return iter->second;
}

const CObjectData2D & CObjectDataMgr::getData2D( const CSpriteData & spriteData ) const
{
    return getData2D( spriteData.getGroup(), spriteData.getObjectName() );
}


/************************************************************************
 *    DESC:  Load all of the meshes and materials of a specific data group
 ************************************************************************/
void CObjectDataMgr::loadGroup3D( const std::string & group )
{
    // Make sure the group we are looking has been defined in the list table file
    auto listTableIter = m_listTableMap.find( group );
    if( listTableIter == m_listTableMap.end() )
        throw NExcept::CCriticalException( "Obj Data List 3D Load Group Data Error!",
            boost::str( boost::format( "Object data list group name can't be found (%s).\n\n%s\nLine: %s" )
                % group % __FUNCTION__ % __LINE__ ) );

    // Load the group data if it doesn't already exist
    if( m_objectData3DMapMap.find( group ) == m_objectData3DMapMap.end() )
    {
        // Create a new group map inside of our map
        m_objectData3DMapMap.emplace( group, std::map<const std::string, CObjectData3D>() );

        for( auto & iter : listTableIter->second )
            load3D( group, iter );
    }
    else
    {
        throw NExcept::CCriticalException( "Obj Data List 3D load Error!",
            boost::str( boost::format( "Object data list group has alread been loaded (%s).\n\n%s\nLine: %s" )
                % group % __FUNCTION__ % __LINE__ ) );
    }
}


/************************************************************************
 *    DESC:  Load all object information
 ************************************************************************/
void CObjectDataMgr::load3D( const std::string & group, const std::string & filePath )
{
    // Open and parse the XML file:
    const XMLNode mainNode = XMLNode::openFileHelper( filePath.c_str(), "objectDataList3D" );

    // Get an iterator to the group
    auto groupMapIter = m_objectData3DMapMap.find( group );

    //////////////////////////////////////////////
    // Load the default data
    //////////////////////////////////////////////

    // Get the node to the default object data
    const XMLNode defaultNode = mainNode.getChildNode( "default" );

    // Class to hold a data list's default data.
    CObjectData3D defaultData;

    // If there's no default node then we just use the defaults set in the data classes
    if( !defaultNode.isEmpty() )
        defaultData.loadFromNode( defaultNode, "", "" );


    //////////////////////////////////////////////
    // Load the object data
    //////////////////////////////////////////////

    XMLNode objectListNode = mainNode.getChildNode( "objectList" );

    for( int i = 0; i < objectListNode.nChildNode(); ++i )
    {
        // Get the object data node
        const XMLNode objectNode = objectListNode.getChildNode( i );

        // Get the object's name
        const std::string name = objectNode.getAttribute( "name" );

        // Allocate the object data to the map
        auto iter = groupMapIter->second.emplace( name, defaultData );

        // Check for duplicate names
        if( !iter.second )
        {
            throw NExcept::CCriticalException( "Object Data Load Group Error!",
                boost::str( boost::format( "Duplicate object name (%s - %s).\n\n%s\nLine: %s" )
                    % name % group % __FUNCTION__ % __LINE__ ) );
        }

        // Load in the object data
        iter.first->second.loadFromNode( objectNode, group, name );
    }
    
    createFromData3D( group );
}


/************************************************************************
 *    DESC:  Create the group's VBO, IBO, textures, etc
 ************************************************************************/
void CObjectDataMgr::createFromData3D( const std::string & group )
{
    // Create it from the data
    auto groupMapIter = m_objectData3DMapMap.find( group );
    if( groupMapIter != m_objectData3DMapMap.end() )
    {
        for( auto & iter : groupMapIter->second )
            iter.second.createFromData( group );
    }
    else
    {
        throw NExcept::CCriticalException("Object Create From Data Group Error!",
            boost::str( boost::format("Object data list group name can't be found (%s).\n\n%s\nLine: %s")
                % group % __FUNCTION__ % __LINE__ ));
    }
}


/************************************************************************
 *    DESC:  Free all of the meshes and materials of a specific data group
 ************************************************************************/
void CObjectDataMgr::freeGroup3D( const std::string & group )
{
    // Free the data of a specific group
    freeDataGroup3D( group );

    // Delete the group assets
    CDevice::Instance().deleteGroupAssets( group );
}


/************************************************************************
 *    DESC:  Free only the data of a specific group
 ************************************************************************/
void CObjectDataMgr::freeDataGroup3D( const std::string & group )
{
    // Make sure the group we are looking for exists
    if( m_listTableMap.find( group ) == m_listTableMap.end() )
        throw NExcept::CCriticalException( "Obj Data List 3D Load Group Data Error!",
            boost::str( boost::format( "Object data list group name can't be found (%s).\n\n%s\nLine: %s" )
                % group % __FUNCTION__ % __LINE__ ) );

    // Unload the group data
    auto groupMapIter = m_objectData3DMapMap.find( group );
    if( groupMapIter != m_objectData3DMapMap.end() )
        m_objectData3DMapMap.erase( groupMapIter );
}


/************************************************************************
 *    DESC:  Get a specific 3D object's data
 ************************************************************************/
const CObjectData3D & CObjectDataMgr::getData3D( const std::string & group, const std::string & name ) const
{
    auto mapIter = m_objectData3DMapMap.find( group );
    if( mapIter == m_objectData3DMapMap.end() )
        throw NExcept::CCriticalException( "Obj Data List 3D Get Data Error!",
            boost::str( boost::format( "Object data list group can't be found (%s).\n\n%s\nLine: %s" )
                % group % __FUNCTION__ % __LINE__ ) );

    auto iter = mapIter->second.find( name );
    if( iter == mapIter->second.end() )
        throw NExcept::CCriticalException( "Obj Data List 3D Get Data Error!",
            boost::str( boost::format( "Object data list name can't be found (%s).\n\n%s\nLine: (%s - %s)" )
                % group % name % __FUNCTION__ % __LINE__ ) );

    return iter->second;
}


/************************************************************************
 *    DESC:  Get a specific 3D object's data
 ************************************************************************/
bool CObjectDataMgr::isData3D( const std::string & group, const std::string & name ) const
{
    auto mapIter = m_objectData3DMapMap.find( group );
    if( mapIter != m_objectData3DMapMap.end() )
    {
        if( mapIter->second.find( name ) != mapIter->second.end() )
            return true;
    }

    return false;
}
