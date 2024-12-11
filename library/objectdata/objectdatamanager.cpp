/************************************************************************
 *    FILE NAME:       objectdatamanager.cpp
 *
 *    DESCRIPTION:    Singlton that holds a map of all 2D/3D object data used for
 *					 later loading
 ************************************************************************/

// Physical component dependency
#include <objectdata/objectdatamanager.h>

// Game lib dependencies
#include <utilities/exceptionhandling.h>
#include <utilities/xmlParser.h>
#include <utilities/settings.h>
#include <objectdata/objectdata2d.h>
#include <objectdata/objectdata3d.h>
#include <managers/spritesheetmanager.h>
#include <system/device.h>

// Standard lib dependencies
#include <string>

// Boost lib dependencies
#include <boost/format.hpp>

// AngelScript lib dependencies
#include <scriptarray/scriptarray.h>

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
 *    DESC:  Get a specific 2D object's data
 ************************************************************************/
const iObjectData & CObjectDataMgr::getData( const std::string & group, const std::string & name ) const
{
    auto mapIter = m_objectDataMapMap.find( group );
    if( mapIter == m_objectDataMapMap.end() )
        throw NExcept::CCriticalException("Obj Data List Get Data Error!",
            boost::str( boost::format("Object data list group can't be found (%s).\n\n%s\nLine: %s")
                % group % __FUNCTION__ % __LINE__ ));

    auto iter = mapIter->second.find( name );
    if( iter == mapIter->second.end() )
        throw NExcept::CCriticalException("Obj Data List Get Data Error!",
            boost::str( boost::format("Object data list name can't be found (%s - %s).\n\n%s\nLine: (%s)")
                % group % name % __FUNCTION__ % __LINE__ ));

    return *iter->second.get();
}


/************************************************************************
 *    DESC:  Load all of the meshes and materials of a specific data group
 ************************************************************************/
void CObjectDataMgr::loadGroupLst( const std::vector<std::string> & groupVec )
{
    for( auto & iter : groupVec )
        loadGroup( iter );
}

void CObjectDataMgr::loadGroupAry( const CScriptArray & strategyIdAry )
{
    for( uint i = 0; i < strategyIdAry.GetSize(); i++ )
        loadGroup( *reinterpret_cast<const std::string *>(strategyIdAry.At(i)) );
}

void CObjectDataMgr::loadGroup( const std::string & group )
{
    // Check for a hardware extension
    std::string ext;
    if( !m_mobileExt.empty() && CSettings::Instance().isMobileDevice() )
        if( m_listTableMap.find( group + m_mobileExt ) != m_listTableMap.end() )
            ext = m_mobileExt;

    // Make sure the group we are looking has been defined in the list table file
    auto listTableIter = m_listTableMap.find( group + ext );
    if( listTableIter == m_listTableMap.end() )
        throw NExcept::CCriticalException("Obj Data List Load Group Data Error!",
            boost::str( boost::format("Object data list group name can't be found (%s).\n\n%s\nLine: %s")
                % group % __FUNCTION__ % __LINE__ ));

    // Load the group data if it doesn't already exist
    if( m_objectDataMapMap.find( group ) == m_objectDataMapMap.end() )
    {
        // Create a new group map inside of our map
        m_objectDataMapMap.emplace( group, std::map<const std::string, std::unique_ptr<iObjectData>>() );

        for( auto & iter : listTableIter->second )
            load( group, iter );
    }
    else
    {
        throw NExcept::CCriticalException("Obj Data List group load Error!",
            boost::str( boost::format("Object data list group has already been loaded (%s).\n\n%s\nLine: %s")
                % group % __FUNCTION__ % __LINE__ ));
    }
}


/************************************************************************
 *    DESC:  Load all object information
 ************************************************************************/
void CObjectDataMgr::load( const std::string & group, const std::string & filePath )
{
    // Open and parse the XML file:
    XMLNode mainNode = XMLNode::openFileHelper( filePath.c_str() );

    if( !mainNode.isEmpty() )
    {
        // if <?xml version="1.0"?> in not specified, will return pointing to the first tag
        // Need to decide is we need to get the child or not
        XMLNode childNode = mainNode;
        if( std::string(childNode.getName()).find("objectDataList") == std::string::npos )
            childNode = mainNode.getChildNode();

        if( std::strcmp( childNode.getName(), "objectDataList2D" ) == 0 )
        {
            load2D( group, childNode );

            // Create the assets from data
            createFromData( group );

            // Free the sprite sheet data because it's no longer needed
            CSpriteSheetMgr::Instance().clear();

            return;
        }

        else if( std::strcmp( childNode.getName(), "objectDataList3D" ) == 0 )
        {
            load3D( group, childNode );

            // Create the assets from data
            createFromData( group );

            return;
        }

        throw NExcept::CCriticalException("Object Data Load Group Error!",
        boost::str( boost::format("Incorrect root tab name (%s - %s).\n\n%s\nLine: %s")
            % group % filePath % __FUNCTION__ % __LINE__ ));
    }

    throw NExcept::CCriticalException("Object Data Load Group Error!",
        boost::str( boost::format("File does not exists (%s - %s).\n\n%s\nLine: %s")
            % group % filePath % __FUNCTION__ % __LINE__ ));
}


/************************************************************************
 *    DESC:  Load all object information
 ************************************************************************/
void CObjectDataMgr::load2D( const std::string & group, const XMLNode & mainNode )
{
    // Get an iterator to the group
    auto groupMapIter = m_objectDataMapMap.find( group );


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
        auto iter = groupMapIter->second.emplace( name, new CObjectData2D( defaultData ) );

        // // Check for duplicate names
        if( !iter.second )
        {
            throw NExcept::CCriticalException("Object Data Load Group Error!",
                boost::str( boost::format("Duplicate object name (%s - %s).\n\n%s\nLine: %s")
                    % name % group % __FUNCTION__ % __LINE__ ));
        }

        // Load in the object data
        iter.first->second->loadFromNode( objectNode, group, name );
    }
}


/************************************************************************
 *    DESC:  Load all object information
 ************************************************************************/
void CObjectDataMgr::load3D( const std::string & group, const XMLNode & mainNode )
{
    // Get an iterator to the group
    auto groupMapIter = m_objectDataMapMap.find( group );

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
        auto iter = groupMapIter->second.emplace( name, new CObjectData3D( defaultData ) );

        // Check for duplicate names
        if( !iter.second )
        {
            throw NExcept::CCriticalException( "Object Data Load Group Error!",
                boost::str( boost::format( "Duplicate object name (%s - %s).\n\n%s\nLine: %s" )
                    % name % group % __FUNCTION__ % __LINE__ ) );
        }

        // Load in the object data
        iter.first->second->loadFromNode( objectNode, group, name );
    }
}


/************************************************************************
 *    DESC:  Create the group's VBO, IBO, textures, etc
 ************************************************************************/
void CObjectDataMgr::createFromData( const std::string & group )
{
    // Create it from the data
    auto groupMapIter = m_objectDataMapMap.find( group );
    if( groupMapIter != m_objectDataMapMap.end() )
    {
        for( auto & iter : groupMapIter->second )
            iter.second->createFromData( group );
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
void CObjectDataMgr::freeGroupLst( const std::vector<std::string> & groupVec )
{
    for( auto & iter : groupVec )
        freeGroup( iter );
}

void CObjectDataMgr::freeGroupAry( const CScriptArray & strategyIdAry )
{
    for( uint i = 0; i < strategyIdAry.GetSize(); i++ )
        freeGroup( *reinterpret_cast<const std::string *>(strategyIdAry.At(i)) );
}

void CObjectDataMgr::freeGroup( const std::string & group )
{
    // Free the data of a specific group
    freeDataGroup( group );
    
    // Delete the group assets
    CDevice::Instance().deleteGroupAssets( group );
}


/************************************************************************
 *    DESC:  Free only the data of a specific group
 ************************************************************************/
void CObjectDataMgr::freeDataGroup( const std::string & group )
{
    // Make sure the group we are looking for exists
    if( m_listTableMap.find( group ) == m_listTableMap.end() )
        throw NExcept::CCriticalException("Obj Data List Free Group Data Error!",
            boost::str( boost::format("Object data list group name can't be found (%s).\n\n%s\nLine: %s")
                % group % __FUNCTION__ % __LINE__ ));

    // Unload the group data
    auto mapIter = m_objectDataMapMap.find( group );
    if( mapIter != m_objectDataMapMap.end() )
        m_objectDataMapMap.erase( mapIter );
}

/************************************************************************
 *    DESC:  Find the group an object name belongs to
 ************************************************************************/
std::string CObjectDataMgr::findGroup( const std::string & objectName )
{
    std::string result;

    // Work your way backwards through the map because chances are we are looking
    // for an object name in a group that was reciently added.
    for( auto iter = m_objectDataMapMap.rbegin(); iter != m_objectDataMapMap.rend(); iter++ )
    {
        if( iter->second.find( objectName ) != iter->second.end() )
        {
            result = iter->first;
            break;
        }
    }

    return result;
}
