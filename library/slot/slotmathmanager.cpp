
/************************************************************************
*    FILE NAME:       slotmathmanager.cpp
*
*    DESCRIPTION:     Singlton for managing slot math data
************************************************************************/

// Physical component dependency
#include <slot/slotmathmanager.h>

// Game lib dependencies
#include <utilities/exceptionhandling.h>
#include <utilities/xmlParser.h>

// Boost lib dependencies
#include <boost/format.hpp>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CSlotMathMgr::CSlotMathMgr()
{
}


/************************************************************************
*    DESC:  destructor
************************************************************************/
CSlotMathMgr::~CSlotMathMgr()
{
}


/************************************************************************
 *    DESC:  Get the slot math
 ************************************************************************/
const CSlotMath & CSlotMathMgr::getSlotMath( const std::string & group, const std::string & id ) const
{
    auto groupMapIter = m_slotMathMapMap.find( group );
    if( groupMapIter == m_slotMathMapMap.end() )
        throw NExcept::CCriticalException("Get Slot Math Data Error!",
            boost::str( boost::format("Slot Math group can't be found (%s).\n\n%s\nLine: %s")
                % group % __FUNCTION__ % __LINE__ ));

    auto dataMapIter = groupMapIter->second.find( id );
    if( dataMapIter == groupMapIter->second.end() )
        throw NExcept::CCriticalException("Get Slot Math Data Error!",
            boost::str( boost::format("Slot Math name can't be found (%s).\n\n%s\nLine: (%s - %s)")
                % group % id % __FUNCTION__ % __LINE__ ));

    return dataMapIter->second;
}


/************************************************************************
 *    DESC:  Load all of the reel group data from a specific group
 ************************************************************************/
void CSlotMathMgr::loadGroup( const std::string & group )
{
    // Make sure the group we are looking has been defined in the list table file
    auto listTableIter = m_listTableMap.find( group );
    if( listTableIter == m_listTableMap.end() )
        throw NExcept::CCriticalException("Symbol config Load Group Data Error!",
            boost::str( boost::format("Symbol config group name can't be found (%s).\n\n%s\nLine: %s")
            % group % __FUNCTION__ % __LINE__ ));

    // Load the group data if it doesn't already exist
    if( m_slotMathMapMap.find( group ) == m_slotMathMapMap.end() )
    {
        // Create a new group map inside of our map
        m_slotMathMapMap.emplace( group, std::map<const std::string, CSlotMath>() );

        load( group, listTableIter->second.back() );
    }
    else
    {
        throw NExcept::CCriticalException("Math config load Error!",
            boost::str( boost::format("Math config group has alread been loaded (%s).\n\n%s\nLine: %s")
            % group % __FUNCTION__ % __LINE__ ));
    }
}


/************************************************************************
 *    DESC:  Load all math data from an xml
 ************************************************************************/
void CSlotMathMgr::load( const std::string & group, const std::string & filePath )
{
    // Open and parse the XML file:
    const XMLNode node = XMLNode::openFileHelper( filePath.c_str(), "math" );

    // Get the id
    const std::string id = node.getAttribute( "id" );

    // Get an iterator to the group
    auto groupMapIter = m_slotMathMapMap.find( group );

    // Allocate the math group data to the map
    auto iter = groupMapIter->second.emplace( id, group );

    // Check for duplicate names
    if( !iter.second )
    {
        throw NExcept::CCriticalException("Math Data Load Group Error",
            boost::str( boost::format("Duplicate math group id (%s - %s).\n\n%s\nLine: %s")
                % id % group % __FUNCTION__ % __LINE__ ));
    }

    // Load the data from node
    iter.first->second.loadFromNode( node );
}


/************************************************************************
*    DESC:  Load the payline configuration from XML file
************************************************************************/
void CSlotMathMgr::loadPaylineSetFromFile( const std::string & filePath )
{
    // Open and parse the XML file:
    const XMLNode node = XMLNode::openFileHelper( filePath.c_str(), "paylineSetList" );

    for( int i = 0; i < node.nChildNode(); ++i )
    {
        const XMLNode paylineNode = node.getChildNode(i);

        // Get the id
        const std::string id = paylineNode.getAttribute( "id" );

        auto iter = m_paylineSetMap.emplace( id, paylineNode );

        // Check for duplicate ids
        if( !iter.second )
        {
            throw NExcept::CCriticalException("Load Payline Config Error!",
                boost::str( boost::format("Duplicate id (%s).\n\n%s\nLine: %s")
                    % id % __FUNCTION__ % __LINE__ ));
        }
    }
}


/************************************************************************
*    DESC:  Get the payline set
************************************************************************/
const CPaylineSet & CSlotMathMgr::getPaylineSet( const std::string & id ) const
{
    // Make sure the group we are looking for exists
    auto iter = m_paylineSetMap.find( id );
    if( iter == m_paylineSetMap.end() )
        throw NExcept::CCriticalException("Payline Set Data Error!",
            boost::str( boost::format("Payline Set id can't be found (%s).\n\n%s\nLine: %s")
                % id % __FUNCTION__ % __LINE__ ));

    return iter->second;
}


/************************************************************************
 *    DESC:  Unload all of the reel group data from a specific group
 ************************************************************************/
void CSlotMathMgr::freeGroup( const std::string & group )
{
    // Make sure the group we are looking for exists
    auto listTableIter = m_listTableMap.find( group );
    if( listTableIter == m_listTableMap.end() )
        throw NExcept::CCriticalException("Obj Data List 2D Free Group Data Error!",
            boost::str( boost::format("Object data list group name can't be found (%s).\n\n%s\nLine: %s")
                % group % __FUNCTION__ % __LINE__ ));

    // See if this group is still loaded
    // Unload the group data
    auto groupMapIter = m_slotMathMapMap.find( group );
    if( groupMapIter != m_slotMathMapMap.end() )
        m_slotMathMapMap.erase( groupMapIter );
}


/************************************************************************
*    DESC:  Free the payline set
************************************************************************/
void CSlotMathMgr::freePaylineSet()
{
    m_paylineSetMap.clear();
}


/************************************************************************
*    DESC:  Clear out all the data
************************************************************************/
void CSlotMathMgr::clear()
{
    m_slotMathMapMap.clear();
    freePaylineSet();
}
