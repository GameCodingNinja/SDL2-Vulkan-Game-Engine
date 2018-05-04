
/************************************************************************
*    FILE NAME:       symbolsetviewmanager.cpp
*
*    DESCRIPTION:     Singleton for managing different symbol view set
************************************************************************/

// Physical component dependency
#include <slot/symbolsetviewmanager.h>

// Game lib dependencies
#include <slot/symbol2d.h>
#include <utilities/xmlParser.h>
#include <utilities/exceptionhandling.h>

// Boost lib dependencies
#include <boost/format.hpp>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CSymbolSetViewMgr::CSymbolSetViewMgr()
{
}


/************************************************************************
*    DESC:  destructor
************************************************************************/
CSymbolSetViewMgr::~CSymbolSetViewMgr()
{
}


/************************************************************************
 *    DESC:  Get the symbol set view
 ************************************************************************/
CSymbolSetView & CSymbolSetViewMgr::get( const std::string & group, const std::string & name )
{
    auto groupMapIter = m_symbolSetViewMap.find( group );
    if( groupMapIter == m_symbolSetViewMap.end() )
        throw NExcept::CCriticalException("Symbol Set Manager Get Set Error!",
            boost::str( boost::format("Symbol set group can't be found (%s).\n\n%s\nLine: %s")
                % group % __FUNCTION__ % __LINE__ ));

    auto dataMapIter = groupMapIter->second.find( name );
    if( dataMapIter == groupMapIter->second.end() )
        throw NExcept::CCriticalException("Symbol Set Manager Get Set Error!",
            boost::str( boost::format("Symbol set name can't be found (%s).\n\n%s\nLine: (%s - %s)")
                % group % name % __FUNCTION__ % __LINE__ ));

    return dataMapIter->second;
}


/************************************************************************
 *    DESC:  Load all of the symbols of a specific group
 ************************************************************************/
void CSymbolSetViewMgr::loadGroup( const std::string & group )
{
    // Make sure the group we are looking has been defined in the list table file
    auto listTableIter = m_listTableMap.find( group );
    if( listTableIter == m_listTableMap.end() )
        throw NExcept::CCriticalException("Symbol config Load Group Data Error!",
            boost::str( boost::format("Symbol config group name can't be found (%s).\n\n%s\nLine: %s")
            % group % __FUNCTION__ % __LINE__ ));

    // Load the group data if it doesn't already exist
    if( m_symbolSetViewMap.find( group ) == m_symbolSetViewMap.end() )
    {
        // Create a new group map inside of our map
        m_symbolSetViewMap.emplace( group, std::map<const std::string, CSymbolSetView>() );

        for( auto & iter : listTableIter->second )
            load( group, iter );
    }
    else
    {
        throw NExcept::CCriticalException("Symbol config load Error!",
            boost::str( boost::format("Symbol config group has alread been loaded (%s).\n\n%s\nLine: %s")
            % group % __FUNCTION__ % __LINE__ ));
    }
}


/************************************************************************
 *    DESC:  Load all symbol information from an xml
 ************************************************************************/
void CSymbolSetViewMgr::load( const std::string & group, const std::string & filePath )
{
    // Open and parse the XML file:
    const XMLNode symbolSetListNode = XMLNode::openFileHelper( filePath.c_str(), "symbolSetList" );

    // Get an iterator to the group
    auto groupMapIter = m_symbolSetViewMap.find( group );

    for( int i = 0; i < symbolSetListNode.nChildNode(); ++i )
    {
        // Get the object data node
        const XMLNode symbolSetNode = symbolSetListNode.getChildNode(i);

        // Get the symbols set name
        const std::string name = symbolSetNode.getAttribute( "name" );

        // Allocate the symbol to the map
        auto iter = groupMapIter->second.emplace( name, group );

        // Check for duplicate names
        if( !iter.second )
        {
            throw NExcept::CCriticalException("Symbol Config Load Group Error!",
                boost::str( boost::format("Duplicate symbol set (%s - %s).\n\n%s\nLine: %s")
                    % name % group % __FUNCTION__ % __LINE__ ));
        }

        // Load in the symbol set data
        iter.first->second.loadFromNode( symbolSetNode, name );
    }
}


/************************************************************************
*    DESC:  Free a symbol group
************************************************************************/
void CSymbolSetViewMgr::freeGroup( const std::string & group )
{
    // Free the texture group if it exists
    auto mapMapIter = m_symbolSetViewMap.find( group );
    if( mapMapIter != m_symbolSetViewMap.end() )
    {
        // Erase this group
        m_symbolSetViewMap.erase( mapMapIter );
    }
}


/************************************************************************
*    DESC:  Clear all the data
************************************************************************/
void CSymbolSetViewMgr::clear()
{
    m_symbolSetViewMap.clear();
}
