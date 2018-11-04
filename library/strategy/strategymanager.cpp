
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
 *    DESC:  Add strategy
 ************************************************************************/
iStrategy * CStrategyMgr::addStrategy( const std::string & strategyId, iStrategy * pSpriteStrategy )
{
    auto mapIter = m_pStrategyMap.emplace( strategyId, pSpriteStrategy );

    // Check for duplicate groups being used
    if( !mapIter.second )
    {
        throw NExcept::CCriticalException("Sprite Manager Strategy Load Error!",
            boost::str( boost::format("Duplicate id name (%s).\n\n%s\nLine: %s")
                % strategyId % __FUNCTION__ % __LINE__ ));
    }

    // See if there is any files associated with the strategy id in the list table
    auto listTableIter = m_listTableMap.find( strategyId );
    if( listTableIter != m_listTableMap.end() )
        for( auto & filePathIter : listTableIter->second )
            mapIter.first->second->loadFromFile( filePathIter );

    // Add the strategy pointer to the vector for rendering
    m_pStrategyVec.push_back( pSpriteStrategy );
    
    return pSpriteStrategy;
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
        auto strategyIter = std::find( m_pStrategyVec.begin(), m_pStrategyVec.end(), mapIter->second );
        if( strategyIter != m_pStrategyVec.end() )
            m_pStrategyVec.erase( strategyIter );

        NDelFunc::Delete( mapIter->second );
        m_pStrategyMap.erase( mapIter );
    }
}


/************************************************************************
 *    DESC:  Delete sprite
 ************************************************************************/
void CStrategyMgr::deleteSprite( const std::string & strategyId, int spriteId )
{
    // Make sure the strategy we are looking for is available
    auto mapIter = m_pStrategyMap.find( strategyId );
    if( mapIter != m_pStrategyMap.end() )
        mapIter->second->setToDestroy( spriteId );
}


/************************************************************************
*    DESC:  create the iNode and provide a unique id number for each one
************************************************************************/
iNode * CStrategyMgr::create(
    const std::string & strategyId,
    const std::string & dataName,
    const CPoint<CWorldValue> & pos,
    const CPoint<float> & rot,
    const CPoint<float> & scale )
{
    // Make sure the strategy we are looking for is available
    auto mapIter = m_pStrategyMap.find( strategyId );
    if( mapIter == m_pStrategyMap.end() )
        throw NExcept::CCriticalException("Sprite Manager Strategy Group Find Error!",
            boost::str( boost::format("Sprite Manager strategy id can't be found (%s, %s).\n\n%s\nLine: %s")
                % strategyId % dataName % __FUNCTION__ % __LINE__ ));

    return mapIter->second->create( dataName, pos, rot, scale );
}

iNode * CStrategyMgr::create(
    const std::string & strategyId,
    const std::string & dataName )
{
    // Make sure the strategy we are looking for is available
    auto mapIter = m_pStrategyMap.find( strategyId );
    if( mapIter == m_pStrategyMap.end() )
        throw NExcept::CCriticalException("Sprite Manager Strategy Group Find Error!",
            boost::str( boost::format("Sprite Manager strategy id can't be found (%s, %s).\n\n%s\nLine: %s")
                % strategyId % dataName % __FUNCTION__ % __LINE__ ));

    return mapIter->second->create( dataName );
}


/************************************************************************
*    DESC:  Delete all the strategies
************************************************************************/
void CStrategyMgr::clear()
{
    NDelFunc::DeleteMapPointers(m_pStrategyMap);
    m_pStrategyMap.clear();
    m_pStrategyVec.clear();
    iStrategy::clearSpriteCounter();
}


/************************************************************************
*    DESC:  Do any pre-game loop init's
************************************************************************/
void CStrategyMgr::init()
{
    for( auto iter : m_pStrategyVec )
        iter->init();
}


/************************************************************************
*    DESC:  Handle any misc processing before the real work is started
************************************************************************/
void CStrategyMgr::miscProcess()
{
    for( auto iter : m_pStrategyVec )
        iter->miscProcess();
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
void CStrategyMgr::recordCommandBuffer( uint32_t index, const CMatrix & viewProj )
{
    for( auto iter : m_pStrategyVec )
        iter->recordCommandBuffer( index, viewProj );
}

void CStrategyMgr::recordCommandBuffer( uint32_t index, const CMatrix & rotMatrix, const CMatrix & viewProj )
{
    for( auto iter : m_pStrategyVec )
        iter->recordCommandBuffer( index, rotMatrix, viewProj );
}


/************************************************************************
*    DESC:  Get the pointer to the strategy
************************************************************************/
iStrategy * CStrategyMgr::getStrategy( const std::string & strategyId )
{
    // Make sure the strategy we are looking for is available
    auto mapIter = m_pStrategyMap.find( strategyId );
    if( mapIter == m_pStrategyMap.end() )
        throw NExcept::CCriticalException("Sprite Strategy Manager Id Get Error!",
            boost::str( boost::format("Sprite Manager strategy Id can't be found (%s).\n\n%s\nLine: %s")
                % strategyId % __FUNCTION__ % __LINE__ ));

    return mapIter->second;
}


/************************************************************************
*    DESC:  Get the pointer to the strategy based on string find
************************************************************************/
iStrategy * CStrategyMgr::findStrategy( const std::string & strategyId )
{
    for( auto & iter : m_pStrategyMap )
        if( iter.first.find(strategyId) != std::string::npos )
            return iter.second;
    
    throw NExcept::CCriticalException("Sprite Strategy Manager Id Find Error!",
        boost::str( boost::format("Sprite Manager strategy Id can't be found via string find (%s).\n\n%s\nLine: %s")
            % strategyId % __FUNCTION__ % __LINE__ ));
    
    return nullptr;
}
