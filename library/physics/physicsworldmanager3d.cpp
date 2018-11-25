
/************************************************************************
*    FILE NAME:       physicsworldmanager3d.cpp
*
*    DESCRIPTION:     Physics manager class singleton
************************************************************************/

// Physical component dependency
#include <physics/physicsworldmanager3d.h>

// Game lib dependencies
#include <utilities/exceptionhandling.h>
#include <utilities/xmlParser.h>
#include <physics/physicsworld3d.h>

// Boost lib dependencies
#include <boost/format.hpp>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CPhysicsWorldManager3D::CPhysicsWorldManager3D()
{
}


/************************************************************************
*    DESC:  destructor
************************************************************************/
CPhysicsWorldManager3D::~CPhysicsWorldManager3D()
{
}


/************************************************************************
*    DESC:  Create the physics world
************************************************************************/
void CPhysicsWorldManager3D::createWorld( const std::string & group )
{
    // Make sure the group we are looking has been defined in the list table file
    auto listTableIter = m_listTableMap.find( group );
    if( listTableIter == m_listTableMap.end() )
        throw NExcept::CCriticalException("Physics 3D Data List Load Group Data Error!",
            boost::str( boost::format("Physics 3D data list group name can't be found (%s).\n\n%s\nLine: %s")
                % group % __FUNCTION__ % __LINE__ ));

    // Load the group data if it doesn't already exist
    if( m_pWorld3dMap.find( group ) == m_pWorld3dMap.end() )
    {
        load( group, listTableIter->second.back() );
    }
    else
    {
        throw NExcept::CCriticalException("Physics world 3D load Error!",
            boost::str( boost::format("Physics world 3D has alread been loaded (%s).\n\n%s\nLine: %s")
                % group % __FUNCTION__ % __LINE__ ));
    }
}


/************************************************************************
*    DESC:  Load the physics worlds from an XML
*
*	 param:	const string & filePath - file to load
************************************************************************/
void CPhysicsWorldManager3D::load( const std::string & group, const std::string & filePath )
{
    // Open and parse the XML file:
    XMLNode mainNode = XMLNode::openFileHelper( filePath.c_str(), "physics3d" );

    // Create the world and add it to the map
    auto iter = m_pWorld3dMap.emplace( std::piecewise_construct, std::forward_as_tuple(group), std::forward_as_tuple() );
    if( !iter.second )
    {
        throw NExcept::CCriticalException("Physics World 3D Manager Error!",
            boost::str( boost::format("World name has already been loaded (%s).\n\n%s\nLine: %s")
                % group % __FUNCTION__ % __LINE__ ));
    }

    // Have the physics world load the rest of its data
    iter.first->second.loadFromNode( mainNode );
}


/************************************************************************
*    DESC:  Get the physics world
************************************************************************/
CPhysicsWorld3D & CPhysicsWorldManager3D::getWorld( const std::string & group )
{
    // Find the world
    auto iter = m_pWorld3dMap.find( group );

    // If we found it, return it. If not, we have a problem
    if( iter == m_pWorld3dMap.end() )
        throw NExcept::CCriticalException("Physics World 3D Manager Error!",
            boost::str( boost::format("Physics World 3D  doesn't exist (%s).\n\n%s\nLine: %s")
                % group.c_str() % __FUNCTION__ % __LINE__ ));

    return iter->second;
}


/************************************************************************
*    DESC:  Destroy the physics world
************************************************************************/
void CPhysicsWorldManager3D::destroyWorld( const std::string & group )
{
    // Find the world
    auto iter = m_pWorld3dMap.find( group );

    // If we find it, delete it. If not, we have a problem
    if( iter != m_pWorld3dMap.end() )
        m_pWorld3dMap.erase( iter );
}


/************************************************************************
*    DESC:  Delete all worlds
************************************************************************/
void CPhysicsWorldManager3D::clear()
{
    m_pWorld3dMap.clear();
}
