
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

// Standard lib dependencies
#include <string>
#include <map>
#include <vector>

// Vulkan lib dependencies
#include <system/vulkan.h>

// Forward Declarations
class iNode;
class iStrategy;
class CMatrix;
union SDL_Event;

class CStrategyMgr : public CManagerBase
{
public:

    // Get the instance of the singleton class
    static CStrategyMgr & Instance()
    {
        static CStrategyMgr spriteStrategyMgr;
        return spriteStrategyMgr;
    }
    
    // Add strategy
    iStrategy * addStrategy( const std::string & strategyId, iStrategy * pSpriteStrategy );
    
    // Delete strategy
    void deleteStrategy( const std::string & strategyId );
    
    // Delete sprite
    void deleteSprite( const std::string & strategyId, int spriteId );
    
    // create the iNode and provide a unique id number for each one
    iNode * create(
        const std::string & strategyId,
        const std::string & dataName,
        const CPoint<CWorldValue> & pos,
        const CPoint<float> & rot = CPoint<float>(),
        const CPoint<float> & scale = CPoint<float>(1,1,1) );
    
    iNode * create(
        const std::string & strategyId,
        const std::string & dataName );

    // Delete all the sprites
    void clear();
    
    // Do any pre-game loop init's
    void init();
    
    // Handle any misc processing before the real work is started.
    void miscProcess();
    
    // Update the sprites
    void update();

    // Transform the sprite
    void transform();
    
    // Record the command buffer for all the sprite objects that are to be rendered
    void recordCommandBuffer( uint32_t index, const CMatrix & viewProj );
    void recordCommandBuffer( uint32_t index, const CMatrix & rotMatrix, const CMatrix & viewProj );
    
    // Get the pointer to the strategy
    iStrategy * getStrategy( const std::string & strategyId );
    
    // Get the pointer to the strategy based on string find
    iStrategy * findStrategy( const std::string & strategyId );
    
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
};

#endif
