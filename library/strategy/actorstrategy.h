
/************************************************************************
*    FILE NAME:       actorstrategy.h
*
*    DESCRIPTION:     Actor strategy class
************************************************************************/

#ifndef __actor_strategy_h__
#define __actor_strategy_h__

// Physical component dependency
#include <strategy/istrategy.h>

// Game lib dependencies
#include <common/point.h>
#include <common/defs.h>
#include <common/worldvalue.h>

// Boost lib dependencies
#include <boost/noncopyable.hpp>

// Standard lib dependencies
#include <string>
#include <vector>
#include <map>
#include <set>

// Forward Declarations
class CNodeDataList;
class CSpriteData;
class CSprite;
class CObject2D;
class CMatrix;

class CActorStrategy : public iStrategy, boost::noncopyable
{
public:

    // Constructor
    CActorStrategy();

    // Destructor
    virtual ~CActorStrategy();

    // Load the node data from file
    void loadFromFile( const std::string & file ) override;

    // Create the node
    iNode * create(
        const std::string & dataName,
        const std::string & instanceName = "",
        bool makeActive = true ) override;
    
    // activate/deactivate node
    iNode * activateNode( const std::string & instanceName );
    void deactivateNode( const std::string & instanceName );
    
    // Destroy the node
    void destroy( int id ) override;

    // Update the nodes
    void update() override;

    // Transform the node
    void transform() override;

    // Record the command buffer for all the sprite objects that are to be rendered
    void recordCommandBuffer( uint32_t index ) override;

    // Get the reference to the node
    template<typename target>
    target * get( const int id )
    {
        return dynamic_cast<target *>(getNode( id ));
    }

    // Find if the node is active
    bool isActive( const int id );
    
    // Get the pointer to the node
    iNode * getNode( const int id );
    iNode * getNode( const std::string & instanceName );

protected:

    // Get the node data by name
    CNodeDataList & getData( const std::string & name );
    
private:
    
    // Add created nodes to the active list
    void addToActiveList();
    
    // Remove deactivated nodes from the active list
    void removeFromActiveList();
    
    // Remove and deleted nodes from the active list and map
    void deleteFromActiveList();

protected:

    // Map of the node data
    std::map<const std::string, CNodeDataList> m_dataMap;

    // Active vector of iNode pointers
    std::vector<iNode *> m_pNodeVec;
    
    // Vector of iNode pointers to be added to the active vector
    std::vector<iNode *> m_pActivateVec;
    
    // Vector of iNode pointers to be removed from the active vector
    std::vector<iNode *> m_pDeactivateVec;
    
    // Set of indexes to delete
    std::vector<int> m_deleteVec;
    
    // Nodes with names
    std::map<const std::string, iNode *> m_pNodeMap;
};

#endif


