
/************************************************************************
*    FILE NAME:       strategymanager.h
*
*    DESCRIPTION:     Strategy manager singleton
************************************************************************/

#pragma once

// Physical component dependency
#include <managers/managerbase.h>

// Standard lib dependencies
#include <string>
#include <map>
#include <vector>
#include <cstdint>

// Forward Declarations
class CStrategy;
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
    CStrategy * addStrategy( const std::string & strategyId, CStrategy * pStrategy );
    
    // activate strategy
    CStrategy * activateStrategy( const std::string & strategyId );
    void activateStrategyLst( const std::vector<std::string> & strategyIdVec );
    void activateStrategyAry( const CScriptArray & strategyIdAry );

    // deactivate strategy
    void deactivateStrategy( const std::string & strategyId );
    void deactivateStrategyLst( const std::vector<std::string> & strategyIdVec );
    void deactivateStrategyAry( const CScriptArray & strategyIdAry );
    void deactivateStrategyPtr( CStrategy * pStrategy );
    
    // Delete strategy
    void deleteStrategy( const std::string & strategyId );
    void deleteStrategyLst( const std::vector<std::string> & strategyIdVec );
    void deleteStrategyAry( const CScriptArray & strategyIdAry );

    // Sort all strategies
    void sort();

    // Delete all the sprites
    void clear();
    
    // Update the sprites
    void update();

    // Transform the sprite
    void transform();
    
    // Record the command buffer for all the sprite objects that are to be rendered
    void recordCommandBuffer( uint32_t index );

    // Update the secondary command buffer vector
    void updateSecondaryCmdBuf( uint32_t index );
    
    // Get the pointer to the strategy
    CStrategy * getStrategy( const std::string & strategyId );

    // Get the strategy vector
    std::vector<CStrategy *> & getStrategyVec()
    { return m_pStrategyVec; }
    
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
    std::map<std::string, CStrategy *> m_pStrategyMap;
    
    // Vector of strategy pointers
    std::vector<CStrategy *> m_pStrategyVec;
};
