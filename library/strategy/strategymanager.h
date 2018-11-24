
/************************************************************************
*    FILE NAME:       strategymanager.h
*
*    DESCRIPTION:     Strategy manager singleton
************************************************************************/

#ifndef __strategy_manager_h__
#define __strategy_manager_h__

// Physical component dependency
#include <managers/managerbase.h>

// Game lib dependencies
#include <common/point.h>
#include <common/worldvalue.h>
#include <common/camera.h>

// Standard lib dependencies
#include <string>
#include <map>
#include <vector>

// Forward Declarations
class iNode;
class CCameraData;
class iStrategy;
class CMatrix;
union SDL_Event;
struct XMLNode;

class CStrategyMgr : public CManagerBase
{
public:

    // Get the instance of the singleton class
    static CStrategyMgr & Instance()
    {
        static CStrategyMgr strategyMgr;
        return strategyMgr;
    }
    
    // Load the group
    void loadGroup( const XMLNode & node, const std::string & strategyId ) override;
    
    // Add strategy
    iStrategy * addStrategy( const std::string & strategyId, iStrategy * pSpriteStrategy );
    
    // Delete strategy
    void deleteStrategy( const std::string & strategyId );
    
    // Set the strategy camera
    void setCamera( const std::string & strategyId, const std::string & cameraId );

    // Delete all the sprites
    void clear();
    
    // Update the sprites
    void update();

    // Transform the sprite
    void transform();
    
    // Record the command buffer for all the sprite objects that are to be rendered
    void recordCommandBuffer( uint32_t index );
    
    // Get the pointer to the strategy
    iStrategy * getStrategy( const std::string & strategyId );
    
    // Build all the camera
    void buildCameras();
    
    // Get a reference to the strategy
    template <typename target>
    target & get( const std::string & strategyId )
    {
        return *dynamic_cast<target *>(getStrategy( strategyId ));
    }
    
    // Get a pointer to the strategy based on a string find
    template <typename target>
    target & find( const std::string & strategyId )
    {
        target * pStrategy = nullptr;
        
        for( auto & iter : m_pStrategyMap )
        {
            pStrategy = dynamic_cast<target *>(iter.second);
            
            if( (pStrategy != nullptr) && (iter.first.find(strategyId) != std::string::npos) )
                return *pStrategy;
            
            pStrategy = nullptr;
        }
        
        return *pStrategy;
    }

private:

    // Constructor
    CStrategyMgr();

    // Destructor
    virtual ~CStrategyMgr();
    
private:

    // Map of strategy pointers
    std::map<std::string, iStrategy *> m_pStrategyMap;
    
    // Vector of strategy pointers
    std::vector<iStrategy *> m_pStrategyVec;
    
    // Camera data for camera creation
    std::map< const std::string, std::map< const std::string, CCameraData > > m_cameraDataMapMap;
    
    // Strategy Camera map
    std::map< const std::string, std::map< const std::string, CCamera > > m_cameraMapMap;
    
    // Default camera
    CCamera m_defaultCamera;
};

#endif
