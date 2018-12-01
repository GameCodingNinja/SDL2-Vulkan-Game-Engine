
/************************************************************************
*    FILE NAME:       basespritestrategy.h
*
*    DESCRIPTION:     Base sprite strategy class
************************************************************************/

#ifndef __base_strategy_h__
#define __base_strategy_h__

// Physical component dependency
#include <strategy/istrategy.h>

// Standard lib dependencies
#include <set>
#include <string>

class CBaseStrategy : public iStrategy
{
public:

    // Constructor
    CBaseStrategy();

    // Destructor
    virtual ~CBaseStrategy(){}
    
    // Set to Destroy the sprite
    void setToDestroy( const int spriteIndex ) override;
    
    // Set to create the sprite
    void setToCreate( const std::string & name ) override;
    
protected:
    
    // Handle the deleting of any object by Id
    virtual void handleDelete() override;
    
    // Handle the deleting of any object by Id
    virtual void deleteObj( const int id );
    
    // Handle the creating of new sprite objects by name
    virtual void handleCreate();
    
    // Handle the creating of any object by name
    virtual void createObj( const std::string & name );
    
private:
    
    // Set of indexes to delete
    // Using a set insures only unique entries are inserted
    std::set<int> m_deleteSet;
    
    // Set of sprites to create
    // Using a set insures only unique entries are inserted
    std::set<std::string> m_createSet;
    
};

#endif
