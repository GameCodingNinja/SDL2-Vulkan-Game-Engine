
/************************************************************************
*    FILE NAME:       strategymanager.cpp
*
*    DESCRIPTION:     Strategy manager singleton
************************************************************************/

// Physical component dependency
#include <strategy/strategymanager.h>

// Game lib dependencies
#include <2d/object2d.h>
#include <utilities/deletefuncs.h>
#include <utilities/exceptionhandling.h>
#include <utilities/matrix.h>
#include <utilities/xmlParser.h>
#include <utilities/settings.h>
#include <utilities/genfunc.h>
#include <strategy/istrategy.h>

// Boost lib dependencies
#include <boost/format.hpp>

// SDL lib dependencies
#include <SDL.h>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CStrategyMgr::CStrategyMgr()
{
}


/************************************************************************
*    DESC:  destructor
************************************************************************/
CStrategyMgr::~CStrategyMgr()
{
    NDelFunc::DeleteMapPointers(m_pStrategyMap);
}


/************************************************************************
 *    DESC:  Add strategy which will load it's data from XML node
 ************************************************************************/
iStrategy * CStrategyMgr::addStrategy( const std::string & strategyId, iStrategy * pSpriteStrategy )
{
    auto mapIter = m_pStrategyMap.emplace( strategyId, pSpriteStrategy );

    // Check for duplicate groups being used
    if( !mapIter.second )
    {
        throw NExcept::CCriticalException("Strategy Manager Load Error!",
            boost::str( boost::format("Duplicate id name (%s).\n\n%s\nLine: %s")
                % strategyId % __FUNCTION__ % __LINE__ ));
    }

    // See if there is any files associated with the strategy id in the list table
    auto listTableIter = m_listTableMap.find( strategyId );
    if( listTableIter != m_listTableMap.end() )
        for( auto & filePathIter : listTableIter->second )
            mapIter.first->second->loadFromFile( filePathIter );
    
    return pSpriteStrategy;
}


/************************************************************************
 *    DESC:  activate strategy
 ************************************************************************/
iStrategy * CStrategyMgr::activateStrategy( const std::string & strategyId )
{
    // Make sure the strategy we are looking for is available
    auto mapIter = m_pStrategyMap.find( strategyId );
    if( mapIter != m_pStrategyMap.end() )
    {
        // See if the strategy is already in the vector
        auto strategyIter = std::find( m_pStrategyVec.begin(), m_pStrategyVec.end(), mapIter->second );
        if( strategyIter != m_pStrategyVec.end() )
            NGenFunc::PostDebugMsg( boost::str( boost::format("Strategy is already active (%s)!") % strategyId ) );
        
        else
            // Add the strategy pointer to the vector for rendering
            m_pStrategyVec.push_back( mapIter->second );
    }
    else
    {
        throw NExcept::CCriticalException("Strategy Manager Activate Error!",
            boost::str( boost::format("Strategy id can't be found (%s).\n\n%s\nLine: %s")
                % strategyId % __FUNCTION__ % __LINE__ ));
    }
    
    return mapIter->second;
}


/************************************************************************
 *    DESC:  deactivate strategy
 ************************************************************************/
void CStrategyMgr::deactivateStrategy( const std::string & strategyId )
{
    // Make sure the strategy we are looking for is available
    auto mapIter = m_pStrategyMap.find( strategyId );
    if( mapIter != m_pStrategyMap.end() )
    {
        // See if the strategy is already in the vector
        auto strategyIter = std::find( m_pStrategyVec.begin(), m_pStrategyVec.end(), mapIter->second );
        if( strategyIter == m_pStrategyVec.end() )
            NGenFunc::PostDebugMsg( boost::str( boost::format("Strategy is not active (%s)!") % strategyId ) );
        
        else
            // Remove the strategy from the render vector
            m_pStrategyVec.erase( strategyIter );
    }
    else
        NGenFunc::PostDebugMsg( boost::str( boost::format("Strategy id can't be found to deactivate (%s)!") % strategyId ) );
}


/************************************************************************
 *    DESC:  Delete strategy
 ************************************************************************/
void CStrategyMgr::deleteStrategy( const std::string & strategyId )
{
    // Make sure the strategy we are looking for is available
    auto mapIter = m_pStrategyMap.find( strategyId );
    if( mapIter != m_pStrategyMap.end() )
    {
        // Delete from the vector
        auto strategyIter = std::find( m_pStrategyVec.begin(), m_pStrategyVec.end(), mapIter->second );
        if( strategyIter != m_pStrategyVec.end() )
            m_pStrategyVec.erase( strategyIter );

        NDelFunc::Delete( mapIter->second );
        m_pStrategyMap.erase( mapIter );
    }
}


/************************************************************************
*    DESC:  Delete all the strategies
************************************************************************/
void CStrategyMgr::clear()
{
    NDelFunc::DeleteMapPointers(m_pStrategyMap);
    m_pStrategyMap.clear();
    m_pStrategyVec.clear();
}


/***************************************************************************
*    DESC:  Update the sprites
****************************************************************************/
void CStrategyMgr::update()
{
    for( auto iter : m_pStrategyVec )
        iter->update();
}


/************************************************************************
*    DESC:  Transform the sprite
************************************************************************/
void CStrategyMgr::transform()
{
    for( auto iter : m_pStrategyVec )
        iter->transform();
}


/***************************************************************************
*    DESC:  Record the command buffer for all the sprite
*           objects that are to be rendered
****************************************************************************/
void CStrategyMgr::recordCommandBuffer( uint32_t index )
{
    for( auto iter : m_pStrategyVec )
        iter->recordCommandBuffer( index );
}


/************************************************************************
*    DESC:  Get the pointer to the strategy
************************************************************************/
iStrategy * CStrategyMgr::getStrategy( const std::string & strategyId )
{
    // Make sure the strategy we are looking for is available
    auto mapIter = m_pStrategyMap.find( strategyId );
    if( mapIter == m_pStrategyMap.end() )
        throw NExcept::CCriticalException("Strategy Manager Id Get Error!",
            boost::str( boost::format("Strategy Id can't be found (%s).\n\n%s\nLine: %s")
                % strategyId % __FUNCTION__ % __LINE__ ));

    return mapIter->second;
}
