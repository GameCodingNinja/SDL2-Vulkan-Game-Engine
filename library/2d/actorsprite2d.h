
/************************************************************************
*    FILE NAME:       actorsprite2d.h
*
*    DESCRIPTION:     2D Actor Sprite Class
************************************************************************/

#ifndef __actor_sprite_2d_h__
#define __actor_sprite_2d_h__

// Physical component dependency
#include <common/isprite.h>
#include <3d/object3d.h>

// Game lib dependencies
#include <common/defs.h>
#include <2d/sprite2d.h>
#include <physics/physicscomponent2d.h>

// Boost lib dependencies
#include <boost/noncopyable.hpp>

// Standard lib dependencies
#include <string>
#include <deque>
#include <map>
#include <memory>

// Forward declaration(s)
class CActorData;

class CActorSprite2D : public iSprite, public CObject3D, boost::noncopyable
{
public:

    // Constructor
    CActorSprite2D( const CActorData & actorData, int id = defs_SPRITE_DEFAULT_ID );

    // Destructor
    virtual ~CActorSprite2D();
    
    // Get the physics component
    CPhysicsComponent2D & getPhysicsComponent();
    
    // Init the sprite
    void init() override;
    
    // Clean up the sprite
    void cleanUp() override;
    
    // Init the physics
    void initPhysics() override;
    
    // React to what the player is doing
    void handleEvent( const SDL_Event & rEvent ) override;

    // Update the actor
    void update() override;
    
    // Update the physics
    void physicsUpdate() override;

    // Transform the actor
    void transform() override;
    void transform( const CObject2D & object ) override;
    void transform( const CMatrix & matrix, bool tranformWorldPos = false ) override;

    // Render the actor
    void render( const CMatrix & matrix ) override;
    
    // Set/Get the AI pointer
    void setAI( iAIBase * pAIBase ) override;

    // Is the actor in view
    bool inView();
    
    // Get the sprite
    CSprite2D & getSprite( int index = 0 );
    
    // Get the sprite group
    CSprite2D & getSprite( const std::string & name );
    
    // Get the collision group
    uint getCollisionGroup() const;
    
    // Get the collision radius
    float getCollisionRadius() const;
    
    // Check for collision against other actor sprite
    bool isCollision( CActorSprite2D & rPlayerActor );
    
    // Get the visual component interface
    iVisualComponent * getVisualInterface() override { return nullptr; }
    
    // Get the physics component interface
    iPhysicsComponent * getPhysicsInterface() override { return nullptr; }
    
protected:
    
    // Apply the scale
    void applyScale( CMatrix & matrix ) override;
    
private:
    
    // Create the actor
    void create( const CActorData & actorData );
    
    // Check if an object is within the orthographic view frustum
    bool inOrthographicView();
    
    // Check if an object is within the perspective view frustum
    bool inPerspectiveView();
    
    // Check for collision against other actor sprite
    bool checkBroadPhase( CActorSprite2D & rPlayerActor );
    
private:
    
    // The physics part of the 2d sprite
    CPhysicsComponent2D m_physicsComponent;
    
    // Base AI scoped pointer
    std::unique_ptr<iAIBase> m_upAI;
    
    // sprite allocation vector
    std::deque<CSprite2D> m_spriteDeq;
    
    // Sprite map for getting sprite pointer by name
    std::map<std::string, CSprite2D *> m_pSpriteMap;
    
    // The radius
    float m_radius;
    
    // The scaled radius
    float m_scaledRadius;
    
    // The projection type
    NDefs::EProjectionType m_projectionType;
    
    // Collision group
    uint m_collisionGroup;
    
    // Collision mask
    uint m_collisionMask;
    
    // Scalar to apply to the radius
    float m_collisionRadiusScalar;
    
    // Collision radius
    float m_collisionRadius;
    
    CColor m_dummy;

};

#endif  // __actor_sprite_2d_h__
