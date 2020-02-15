
/************************************************************************
*    FILE NAME:       strategymanager.h
*
*    DESCRIPTION:     Strategy manager singleton
************************************************************************/

#ifndef __strategy_manager_h__
#define __strategy_manager_h__

// Physical component dependency
#include <managers/managerbase.h>

// Standard lib dependencies
#include <string>
#include <map>
#include <vector>

// Forward Declarations
class iStrategy;
class CScriptArray;

class CStrategyMgr : public CManagerBase
{
public:

    // Get the instance of the singleton class
    static CStrategyMgr & Instance()
    {
        static CStrategyMgr strategyMgr;
        return strategyMgr;
    }
    
    // Add strategy which will load it's data from XML node
    iStrategy * addStrategy( const std::string & strategyId, iStrategy * pSpriteStrategy );
    
    // activate strategy
    iStrategy * activateStrategy( const std::string & strategyId );
    void activateStrategyLst( const std::vector<std::string> & strategyIdVec );
    void activateStrategyAry( const CScriptArray & strategyIdAry );

    // deactivate strategy
    void deactivateStrategy( const std::string & strategyId );
    void deactivateStrategyLst( const std::vector<std::string> & strategyIdVec );
    void deactivateStrategyAry( const CScriptArray & strategyIdAry );
    
    // Delete strategy
    void deleteStrategy( const std::string & strategyId );
    void deleteStrategyLst( const std::vector<std::string> & strategyIdVec );
    void deleteStrategyAry( const CScriptArray & strategyIdAry );

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
