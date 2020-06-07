
/************************************************************************
*    FILE NAME:       actorstrategy.h
*
*    DESCRIPTION:     Actor strategy class
************************************************************************/

#ifndef __actor_strategy_h__
#define __actor_strategy_h__

// Physical component dependency
#include <strategy/istrategy.h>

// Boost lib dependencies
#include <boost/noncopyable.hpp>

// Standard lib dependencies
#include <string>
#include <vector>
#include <map>

// Forward Declarations
class CNodeDataList;

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
        bool makeActive = true,
        const std::string & group = std::string() ) override;
    
    // activate/deactivate node
    iNode * activateNode( const std::string & instanceName ) override;
    void deactivateNode( const std::string & instanceName ) override;
    
    // Destroy the node
    void destroy( int id ) override;

    // Update the nodes
    void update() override;

    // Transform the node
    void transform() override;

    // Record the command buffer for all the sprite objects that are to be rendered
    void recordCommandBuffer( uint32_t index ) override;

    // Find if the node is active
    bool isActive( const int id );
    
    // Get the pointer to the node
    iNode * getNode( const int id );
    iNode * getNode( const std::string & instanceName ) override;

    // Clear all nodes
    void clear() override;

protected:

    // Get the node data by name
    CNodeDataList & getData( const std::string & name, const std::string & _group = std::string() );
    
private:
    
    // Add created nodes to the active list
    void addToActiveList();
    
    // Remove deactivated nodes from the active list
    void removeFromActiveList();
    
    // Remove and deleted nodes from the active list and map
    void deleteFromActiveList();

    // Clear all nodes
    void clearAllNodes();

protected:

    // Map of the node data
    std::map<const std::string, CNodeDataList> m_dataMap;

    // Active vector of iNode pointers
    std::vector<iNode *> m_pNodeVec;

    // Nodes with names
    std::map<const std::string, iNode *> m_pNodeMap;
    
    // Vector of iNode pointers to be added to the active vector
    std::vector<iNode *> m_pActivateVec;
    
    // Vector of iNode pointers to be removed from the active vector
    std::vector<iNode *> m_pDeactivateVec;
    
    // Set of indexes to delete
    std::vector<int> m_deleteVec;

    // Clear all nodes flag
    bool m_clearAllNodesFlag = false;
};

#endif
