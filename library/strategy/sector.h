
/************************************************************************
*    FILE NAME:       sector2d.h
*
*    DESCRIPTION:     Class the creates & renders all the sector nodes
************************************************************************/

#ifndef __sector_h__
#define __sector_h__

// Physical component dependency
#include <common/objecttransform.h>

// Boost lib dependencies
#include <boost/noncopyable.hpp>

// Standard lib dependencies
#include <vector>
#include <map>

// Vulkan lib dependencies
#include <system/vulkan.h>

// Forward Declarations
class CSprite;
class CCamera;
class iNode;
class CSpriteData;

class CSector : public CObjectTransform, boost::noncopyable
{
public:

    // Constructor
    CSector();

    // Destructor
    virtual ~CSector();
    
    // Load the sector data from node
    void loadFromNode( const struct XMLNode & node );

    // Update the actor
    void update();

    // Transform the actor
    void transform() override;
    
    // Record the command buffer for all the node objects that are to be rendered
    void recordCommandBuffer( uint32_t index, VkCommandBuffer cmdBuffer, const CCamera & camera );
    
    // Remove
    void render( const CCamera & camera );
    void render( const CMatrix & matrix );
    void render( const CMatrix & matrix, const CMatrix & rotMatrix );

    // Is the actor in view
    bool inView();
    
    // Get the pointer to the node
    iNode * get( const std::string & nodeName );

private:
    
    // Load the node
    void loadSprite( CSprite * sprite, const CSpriteData & rSpriteData );
    
    // Check if the sector is within the orthographic view frustum
    bool inOrthographicView();
    
    // Check if the sector is within the perspective view frustum
    bool inPerspectiveView();
    
private:
    
    // Vector of iNode pointers
    std::vector<iNode *> m_pNodeVec;
    
    // Nodes with names
    std::map<const std::string, iNode *> m_pNodeMap;
    
    // The projection type
    NDefs::EProjectionType m_projectionType;
    
    // Half of the sector size
    float m_sectorSizeHalf;
};

#endif


