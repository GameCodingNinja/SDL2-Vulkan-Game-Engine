
/************************************************************************
*    FILE NAME:       isprite.h
*
*    DESCRIPTION:     iSprite Class
************************************************************************/

#ifndef __i_sprite_h__
#define __i_sprite_h__

// Physical component dependency
#include <2d/object2d.h>
#include <common/color.h>

// Forward declaration(s)
union SDL_Event;
class iAIBase;
class CCamera;
class CMatrix;
class iVisualComponent;
class iPhysicsComponent;

class iSprite : public virtual CObject2D
{
public:
    
    // Constructor
    iSprite( int id );
    
    // Init the sprite
    virtual void init(){};
    
    // Clean up the sprite
    virtual void cleanUp(){};
    
    // Init the physics
    virtual void initPhysics() = 0;
    
    // React to what the player is doing
    virtual void handleEvent( const SDL_Event & rEvent ) = 0;

    // Update the sprite 
    virtual void update() = 0;
    
    // Update the physics 
    virtual void physicsUpdate() = 0;
    
    // Get the visual component interface
    virtual iVisualComponent * getVisualInterface() = 0;
    
    // Get the physics component interface
    virtual iPhysicsComponent * getPhysicsInterface() = 0;
    
    // do the render
    virtual void render( const CCamera & camera ){};
    virtual void render( const CMatrix & matrix ){};
    virtual void render( const CMatrix & matrix, const CMatrix & rotMatrix ){};
    
    // Get the unique id number
    int getId() const;
    
    // Set/Get the AI pointer
    virtual void setAI( iAIBase * pAIBase ) = 0;
    
    // Get the frame count
    virtual uint getFrameCount() const { return 0; }

    // Set the texture ID from index
    virtual void setFrame( uint index ) {}
    
    // Get the current frame
    virtual uint getCurrentFrame() const { return 0; }

protected:
    
    // Unique Id number
    int m_id;
};

#endif
