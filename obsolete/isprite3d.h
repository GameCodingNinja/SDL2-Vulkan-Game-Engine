
/************************************************************************
*    FILE NAME:       isprite3d.h
*
*    DESCRIPTION:     iSprite3D Class
************************************************************************/

#ifndef __i_sprite_3d_h__
#define __i_sprite_3d_h__

// Physical component dependency
#include <3d/object3d.h>

// Forward declaration(s)
union SDL_Event;
class iAIBase2D;
class CPhysicsComponent3D;

class iSprite3D : public CObject3D
{
public:
    
    // Init the physics
    virtual void InitPhysics() = 0;
    
    // React to what the player is doing
    virtual void HandleEvent( const SDL_Event & rEvent ) = 0;

    // Update the sprite 
    virtual void Update() = 0;
    
    // Update the physics 
    //virtual void PhysicsUpdate() = 0;

    // do the render
    virtual void Render( const CMatrix & projMatrix, const CMatrix & cameraMatrix ) = 0;
    
    // Get the unique id number
    //virtual int GetId() const = 0;
    
    // Set/Get the AI pointer
    virtual void SetAI( iAIBase2D * pAIBase ) = 0;
    
    // Get the physics component
    virtual CPhysicsComponent3D & GetPhysicsComponent() = 0;
    
    // Is this a 3D sprite?
    bool IsSprite3D();
    
    // Is this a 3D actor?
    bool IsActor3D();
};

#endif  // __i_sprite_3d_h__
