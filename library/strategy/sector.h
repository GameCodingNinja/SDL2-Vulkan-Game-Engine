
/************************************************************************
*    FILE NAME:       sector2d.h
*
*    DESCRIPTION:     Class the creates & renders all the sector sprites
************************************************************************/

#ifndef __sector_h__
#define __sector_h__

// Physical component dependency
#include <3d/object3d.h>

// Boost lib dependencies
#include <boost/noncopyable.hpp>

// Standard lib dependencies
#include <vector>
#include <map>

// Forward Declarations
class CSprite;
class CCamera;

class CSector : public CObject3D, boost::noncopyable
{
public:

    // Constructor
    CSector();

    // Destructor
    virtual ~CSector();
    
    // Load the sector data from node
    void loadFromNode( const struct XMLNode & node );
    
    // Do any pre-game loop init's
    void init();
    
    // Do some cleanup
    void cleanUp();
    
    // Destroy this sector
    void destroy();

    // Update the actor
    void update();

    // Transform the actor
    void transform() override;
    void transform( const CObject2D & object ) override;

    // Render the actor
    void render( const CCamera & camera );
    void render( const CMatrix & matrix );
    void render( const CMatrix & matrix, const CMatrix & rotMatrix );

    // Is the actor in view
    bool inView();
    
    // Find if the sprite exists
    bool find( CSprite * pSprite );
    
    // Get the pointer to the sprite
    CSprite * get( const std::string & spriteName );
    
private:
    
    // Check if the sector is within the orthographic view frustum
    bool inOrthographicView();
    
    // Check if the sector is within the perspective view frustum
    bool inPerspectiveView();
    
private:
    
    // sprite allocation vector
    std::vector<CSprite *> m_pSpriteVec;
    
    // sprites with names
    std::map<const std::string, CSprite *> m_pSpriteMap;
    
    // The projection type
    NDefs::EProjectionType m_projectionType;
    
    // Half of the sector size
    float m_sectorSizeHalf;

};

#endif  // __sector_h__


