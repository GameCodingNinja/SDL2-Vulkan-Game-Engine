
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
#include <common/cameradata.h>

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
*    DESC:  Init the default camera
************************************************************************/
void CStrategyMgr::initDefaultCamera()
{
    m_defaultCamera.init(
        CSettings::Instance().getProjectionType(),
        CSettings::Instance().getViewAngle(),
        CSettings::Instance().getMinZdist(),
        CSettings::Instance().getMaxZdist() );
    
    // Set the default position which allows everything to render
    m_defaultCamera.setPos( 0, 0, 100 );
    m_defaultCamera.transform();
}


/************************************************************************
*    DESC:  Load the group
************************************************************************/
void CStrategyMgr::loadGroup( const XMLNode & node, const std::string & strategyId )
{
    for( int i = 0; i < node.nChildNode("camera"); ++i )
    {
        const XMLNode cameraNode = node.getChildNode("camera", i);
        
        // Create the map strategy id if it doesn't already exist
        auto mapIter = m_cameraDataMapMap.find( strategyId );
        if( mapIter == m_cameraDataMapMap.end() )
            mapIter = m_cameraDataMapMap.emplace( strategyId, std::map<const std::string, CCameraData>() ).first;

        // Get the camera id
        std::string cameraId = "";
        if( cameraNode.isAttributeSet( "id" ) )
            cameraId = cameraNode.getAttribute( "id" );

        // Must have a camera id
        if( cameraId.empty() )
        {
            throw NExcept::CCriticalException("Strategy Manager camera data load error!",
                boost::str( boost::format("Camera does not have a id (%s).\n\n%s\nLine: %s")
                    % strategyId % __FUNCTION__ % __LINE__ ));
        }

        // Load the camera data
        auto iter = mapIter->second.emplace( cameraId, cameraNode );

        // Check for duplicate names
        if( !iter.second )
        {
            throw NExcept::CCriticalException("Strategy Manager camera data load error!",
                boost::str( boost::format("Duplicate camera id (%s).\n\n%s\nLine: %s")
                    % cameraId % __FUNCTION__ % __LINE__ ));
        }
    }
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
        throw NExcept::CCriticalException("Strategy Manager Load Error!",
            boost::str( boost::format("Duplicate id name (%s).\n\n%s\nLine: %s")
                % strategyId % __FUNCTION__ % __LINE__ ));
    }
    
    // See if we need to make some cameras
    auto cameraDataMapIter = m_cameraDataMapMap.find( strategyId );
    if( cameraDataMapIter != m_cameraDataMapMap.end() )
    {
        // Make an entry into the map
        auto iter = m_cameraMapMap.emplace( strategyId, std::map< const std::string, CCamera >() );
        
        // Create each camera with the data
        for( auto & cameraDataIter : cameraDataMapIter->second )
            iter.first->second.emplace( cameraDataIter.first, cameraDataIter.second );
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
        // Delete from the vector
        auto strategyIter = std::find( m_pStrategyVec.begin(), m_pStrategyVec.end(), mapIter->second );
        if( strategyIter != m_pStrategyVec.end() )
            m_pStrategyVec.erase( strategyIter );

        NDelFunc::Delete( mapIter->second );
        m_pStrategyMap.erase( mapIter );
    }
    
    // Delete it's associated cameras
    auto cameraMapIter = m_cameraMapMap.find( strategyId );
    if( cameraMapIter != m_cameraMapMap.end() )
        m_cameraMapMap.erase( cameraMapIter );
}


/************************************************************************
 *    DESC:  Set the strategy camera
 ************************************************************************/
void CStrategyMgr::setCamera( const std::string & strategyId, const std::string & cameraId )
{
    auto cameraMapIter = m_cameraMapMap.find( strategyId );
    if( cameraMapIter != m_cameraMapMap.end() )
    {
        auto cameraIter = cameraMapIter->second.find( cameraId );
        if( cameraIter != cameraMapIter->second.end() )
        {
            getStrategy( strategyId )->setCamera( &cameraIter->second );
        }
        else
        {
            NGenFunc::PostDebugMsg( boost::str( boost::format("Strategy doesn't have camera defined (%s, %s).") % strategyId % cameraId ) );
        }
    }
    else
    {
        NGenFunc::PostDebugMsg( boost::str( boost::format("Strategy doesn't have any camera's defined (%s).") % strategyId ) );
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
    iStrategy::clearSpriteCounter();
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
        iter->recordCommandBuffer( index, m_defaultCamera );
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
*    DESC:  Build all the camera
************************************************************************/
void CStrategyMgr::buildCameras()
{
    for( auto & cameraMapIter : m_cameraMapMap )
        for( auto & cameraIter : cameraMapIter.second )
            cameraIter.second.buildProjectionMatrix();
}
