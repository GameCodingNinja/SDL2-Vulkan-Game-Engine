
/************************************************************************
*    FILE NAME:       isprite2d.h
*
*    DESCRIPTION:     iSprite2D Class
************************************************************************/

#ifndef __i_sprite_2d_h__
#define __i_sprite_2d_h__

// Physical component dependency
#include <2d/object2d.h>

// Forward declaration(s)
union SDL_Event;
class iAIBase2D;
class CPhysicsComponent2D;

class iSprite2D : public CObject2D
{
public:
    
    // Init the sprite
    virtual void Init() = 0;
    
    // Clean up the sprite
    virtual void CleanUp() = 0;
    
    // Init the physics
    virtual void InitPhysics() = 0;
    
    // React to what the player is doing
    virtual void HandleEvent( const SDL_Event & rEvent ) = 0;

    // Update the sprite 
    virtual void Update() = 0;
    
    // Update the physics 
    virtual void PhysicsUpdate() = 0;

    // do the render
    virtual void Render( const CMatrix & matrix ) = 0;
    
    // Get the unique id number
    virtual int GetId() const = 0;
    
    // Set/Get the AI pointer
    virtual void SetAI( iAIBase2D * pAIBase ) = 0;
    
    // Get the physics component
    virtual CPhysicsComponent2D & GetPhysicsComponent() = 0;
    
    // Is this a 2D sprite?
    bool IsSprite2D();
    
    // Is this a 2D actor?
    bool IsActor2D();
};

#endif  // __i_sprite_2d_h__
