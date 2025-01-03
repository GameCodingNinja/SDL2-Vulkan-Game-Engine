
/************************************************************************
*    FILE NAME:       strategy.h
*
*    DESCRIPTION:     Strategy class
************************************************************************/

#pragma once

// Physical component dependency
#include <common/object.h>

// Game lib dependencies
#include <common/worldvalue.h>

// Vulkan lib dependencies
#include <system/vulkan.h>

// Standard lib dependencies
#include <string>
#include <vector>
#include <map>

// Forward Declarations
class CNodeDataList;
class iNode;
class CCamera;

class CStrategy : public CObject
{
public:

    // Constructor
    CStrategy();

    // Destructor
    virtual ~CStrategy();

    // Load the node data from file
    void loadFromFile( const std::string & file );

    // Create the node
    iNode * create(
        const std::string & dataName,
        const std::string & instanceName = "",
        bool makeActive = true,
        const std::string & group = std::string() );
    
    // activate/deactivate node
    iNode * activateNode( const std::string & instanceName );
    void deactivateNode( const std::string & instanceName );

    // Get the world value position
    const CPoint<CWorldValue> & getWorldValuePos() const;

    // Set/Inc the world value position
    void setPos( const CPoint<CWorldValue> & position );
    void setPos( CWorldValue x = 0, CWorldValue y = 0, CWorldValue z = 0 );

    void incPos( const CPoint<CWorldValue> & position );
    void incPos( CWorldValue x = 0, CWorldValue y = 0, CWorldValue z = 0 );
    
    // Destroy the node
    void destroy( const handle16_t handle );

    // Update the nodes
    void update();

    // Transform the node
    void transform();

    // Set the command buffers
    void setCommandBuffers( std::vector<VkCommandBuffer> & commandBufVec );

    // Record the command buffer for all the sprite objects that are to be rendered
    void recordCommandBuffer( uint32_t index );

    // Find if the node is active
    bool isActive( const handle16_t handle );
    
    // Get the pointer to the node
    iNode * getNode( const std::string & instanceName );

    // Set to create the sprite
    void setCamera( const std::string & cameraId );
    CCamera & getCamera();

    // Clear all nodes
    void clear();

    // Update the secondary command buffer vector
    void updateSecondaryCmdBuf( uint32_t index );

    // Set the extra camera
    void setExtraCamera( CCamera * pCamera );

    // Increment tha active node vector position of all elements  
    void incActiveVecPos( const float x = 0.f, const float y = 0.f, float z = 0.f );

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

    // World position value
    CPoint<CWorldValue> m_worldValPos;
    
    // Default camera pointer
    CCamera * m_pCamera;

    // Extra camera
    CCamera * m_extraCamera;

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
    
    // Set of handles to delete
    std::vector<handle16_t> m_deleteVec;

    // Clear all vector
    std::vector<iNode *> m_clearAllVec;

    // Clear all nodes flag
    bool m_clearAllNodesFlag = false;

    // Command buffer
    // NOTE: command buffers don't have to be freed because
    //       they are freed by deleting the pool they belong to
    //       and the pool will be freed at the end of the state
    std::vector<VkCommandBuffer> m_commandBufVec;
};
