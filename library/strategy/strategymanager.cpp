
/************************************************************************
*    FILE NAME:       strategymanager.cpp
*
*    DESCRIPTION:     Strategy manager singleton
************************************************************************/

// Physical component dependency
#include <strategy/strategymanager.h>

// Game lib dependencies
#include <utilities/deletefuncs.h>
#include <utilities/exceptionhandling.h>
#include <utilities/genfunc.h>
#include <strategy/strategy.h>

// Boost lib dependencies
#include <boost/format.hpp>

// AngelScript lib dependencies
#include <scriptarray/scriptarray.h>

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
CStrategy * CStrategyMgr::addStrategy( const std::string & strategyId, CStrategy * pStrategy )
{
    auto mapIter = m_pStrategyMap.emplace( strategyId, pStrategy );

    // Check for duplicate groups being used
    if( !mapIter.second )
    {
        throw NExcept::CCriticalException("Strategy Manager Load Error!",
            boost::str( boost::format("Duplicate id name (%s).\n\n%s\nLine: %s")
                % strategyId % __FUNCTION__ % __LINE__ ));
    }

    // See if there is any files associated with the strategy in the list table
    // NOTE: Will return an empty strategy if a file is not defined. Will do an object 
    // data search to create a node/sprite. Assumes sprite only.
    auto listTableIter = m_listTableMap.find( strategyId );
    if( listTableIter != m_listTableMap.end() )
    {
        for( auto & filePathIter : listTableIter->second )
            mapIter.first->second->loadFromFile( filePathIter );
    }
    
    return pStrategy;
}

/************************************************************************
 *    DESC:  activate strategy
 ************************************************************************/
void CStrategyMgr::activateStrategyLst( const std::vector<std::string> & strategyIdVec )
{
    for( auto & iter : strategyIdVec )
        activateStrategy( iter );
}

void CStrategyMgr::activateStrategyAry( const CScriptArray & strategyIdAry )
{
    for( uint i = 0; i < strategyIdAry.GetSize(); i++ )
        activateStrategy( *reinterpret_cast<const std::string *>(strategyIdAry.At(i)) );
}

CStrategy * CStrategyMgr::activateStrategy( const std::string & strategyId )
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
void CStrategyMgr::deactivateStrategyLst( const std::vector<std::string> & strategyIdVec )
{
    for( auto & iter : strategyIdVec )
        deactivateStrategy( iter );
}

void CStrategyMgr::deactivateStrategyAry( const CScriptArray & strategyIdAry )
{
    for( uint i = 0; i < strategyIdAry.GetSize(); i++ )
        deactivateStrategy( *reinterpret_cast<const std::string *>(strategyIdAry.At(i)) );
}

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
            // Remove the strategy from the active vector
            m_pStrategyVec.erase( strategyIter );
    }
    else
        NGenFunc::PostDebugMsg( boost::str( boost::format("Strategy id can't be found to deactivate (%s)!") % strategyId ) );
}

void CStrategyMgr::deactivateStrategyPtr( CStrategy * pStrategy )
{
    // Remove the strategy from the active vector
    auto strategyIter = std::find( m_pStrategyVec.begin(), m_pStrategyVec.end(), pStrategy );
    if( strategyIter != m_pStrategyVec.end() )
        m_pStrategyVec.erase( strategyIter );
}

/************************************************************************
 *    DESC:  Delete strategy
 ************************************************************************/
void CStrategyMgr::deleteStrategyLst( const std::vector<std::string> & strategyIdVec )
{
    for( auto & iter : strategyIdVec )
        deleteStrategy( iter );
}

void CStrategyMgr::deleteStrategyAry( const CScriptArray & strategyIdAry )
{
    for( uint i = 0; i < strategyIdAry.GetSize(); i++ )
        deleteStrategy( *reinterpret_cast<const std::string *>(strategyIdAry.At(i)) );
}

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
    else
        NGenFunc::PostDebugMsg( boost::str( boost::format("Strategy id can't be found to delete (%s)!") % strategyId ) );
}

/************************************************************************
*    DESC:  Sort all strategies
************************************************************************/
void CStrategyMgr::sort()
{
    std::sort(m_pStrategyVec.begin(), m_pStrategyVec.end(),
        [](CStrategy * a, CStrategy * b)
        {
            return a->getPos().z > b->getPos().z;
        });
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

/***************************************************************************
*    DESC:  Update the secondary command buffer vector
****************************************************************************/
void CStrategyMgr::updateSecondaryCmdBuf( uint32_t index )
{
    for( auto iter : m_pStrategyVec )
        iter->updateSecondaryCmdBuf( index );
}

/************************************************************************
*    DESC:  Get the pointer to the strategy
************************************************************************/
CStrategy * CStrategyMgr::getStrategy( const std::string & strategyId )
{
    // Make sure the strategy we are looking for is available
    auto mapIter = m_pStrategyMap.find( strategyId );
    if( mapIter == m_pStrategyMap.end() )
        throw NExcept::CCriticalException("Strategy Manager Id Get Error!",
            boost::str( boost::format("Strategy Id can't be found (%s).\n\n%s\nLine: %s")
                % strategyId % __FUNCTION__ % __LINE__ ));

    return mapIter->second;
}
