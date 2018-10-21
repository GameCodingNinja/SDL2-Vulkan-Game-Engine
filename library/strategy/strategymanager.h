
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
#include <unordered_map>
#include <vector>

// Forward Declarations
class iNode;
class iStrategy;
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
    
    // Do some cleanup
    void cleanUp();
    
    // Handle any misc processing before the real work is started.
    void miscProcess();
    
    // Update the sprites
    void update();

    // Transform the sprite
    void transform();
    void transform( const class CObject2D & object );

    // Render the sprites
    void render();
    void render( const class CMatrix & matrix );
    void render( const CMatrix & matrix, const CMatrix & cameraMatrix );
    
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
    
    // Get a pointer to the strategy based on if the sprite can be found
    /*template <typename target>
    target & find( CSprite * pSprite )
    {
        target * pStrategy = nullptr;
        
        for( auto iter : m_pStrategyVec )
        {
            pStrategy = dynamic_cast<target *>(iter);
            
            if( (pStrategy != nullptr) && pStrategy->Find(pSprite) )
                return *pStrategy;
            
            pStrategy = nullptr;
        }
        
        return *pStrategy;
    }*/
    
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

#endif  // __sprite_strategy_manager_h__


