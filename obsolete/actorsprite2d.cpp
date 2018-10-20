
/************************************************************************
*    FILE NAME:       actorsprite2d.cpp
*
*    DESCRIPTION:     2D Actor Sprite Class
************************************************************************/

// Physical component dependency
#include <2d/actorsprite2d.h>

// Game lib dependencies
#include <utilities/exceptionhandling.h>
#include <utilities/settings.h>
#include <common/actordata.h>
#include <common/size.h>
#include <common/iaibase.h>
#include <objectdata/objectdatamanager.h>
#include <objectdata/objectdata2d.h>

// Boost lib dependencies
#include <boost/format.hpp>

// SDL lib dependencies
#include <SDL.h>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CActorSprite2D::CActorSprite2D( const CActorData & actorData, int id ) :
    iSprite(id),
    m_radius(0),
    m_scaledRadius(0),
    m_projectionType(CSettings::Instance().getProjectionType()),
    m_collisionGroup(0),
    m_collisionMask(0)
{
    // Create the actor based on the data
    create( actorData );

}   // constructor


/************************************************************************
*    DESC:  destructor
************************************************************************/
CActorSprite2D::~CActorSprite2D()
{
}   // destructor


/************************************************************************
*    DESC:  Set/Get the AI pointer. This class owns the pointer
************************************************************************/
void CActorSprite2D::setAI( iAIBase * pAIBase )
{
    m_upAI.reset(pAIBase);

    // Handle any initialization in a separate function
    m_upAI->init();

}   // SetAI


/************************************************************************
*    DESC:  Create the actor's sprites
************************************************************************/
void CActorSprite2D::create( const CActorData & actorData )
{
    const auto & spriteDataVec = actorData.getSpriteData();

    CSize<float> largestSize;

    for( auto & iter: spriteDataVec )
    {
        // Allocate the sprite and add it to the map for easy access
        m_spriteDeq.emplace_back( CObjectDataMgr::Instance().getData2D( iter ), iter.getId() );
        
        // If there's a name for this sprite, add it to the map
        if( !iter.getName().empty() )
        {
            auto duplicateCheckIter = m_pSpriteMap.emplace( iter.getName(), &m_spriteDeq.back() );
            
            // Check for duplicate names
            if( !duplicateCheckIter.second )
            {
                throw NExcept::CCriticalException("Actor Sprite Creation !rror!",
                    boost::str( boost::format("Duplicate sprite name (%s).\n\n%s\nLine: %s")
                        % iter.getName() % __FUNCTION__ % __LINE__ ));
            }
        }
        
        // Copy over the transform for the sprite
        m_spriteDeq.back().copyTransform( &iter );

        // Find the largest size width and height of the different sprites for the controls size
        const CSize<float> & size = m_spriteDeq.back().getObjectData().getSize();
        const CPoint<CWorldValue> & pos = m_spriteDeq.back().getPos();
        const CPoint<float> & scale = m_spriteDeq.back().getScale();

        const float width( (size.w + std::fabs(pos.x)) * scale.x );
        const float height( (size.h + std::fabs(pos.y)) * scale.y );

        // Find the largest size to use as radius
        if( width > largestSize.w )
            largestSize.w = width;

        if( height > largestSize.h )
            largestSize.h = height;
    }
    
    // Copy over the transform for the Actor sprite
    copyTransform( &actorData );

    // Convert the largest width and height to a radius
    largestSize /= 2;
    m_radius = largestSize.getLength();
    m_scaledRadius = m_radius;
    
    // Set the collision filter info
    m_collisionGroup = actorData.getCollisionGroup();
    m_collisionMask = actorData.getCollisionMask();
    m_collisionRadiusScalar = actorData.getCollisionRadiusScalar();
    
    // Init the radius for collision
    m_collisionRadius = m_scaledRadius * m_collisionRadiusScalar;

}   // Create


/************************************************************************
*    DESC:  Init the sprite
*           NOTE: Do not call from a constructor!
************************************************************************/
void CActorSprite2D::init()
{
    for( auto & iter : m_spriteDeq )
        iter.init();
    
} // Init


/************************************************************************
*    DESC:  Clean up the sprite
*           NOTE: Do not call from a destructor!
************************************************************************/
void CActorSprite2D::cleanUp()
{
    for( auto & iter : m_spriteDeq )
        iter.cleanUp();
    
} // CleanUp


/************************************************************************
*    DESC:  Init the physics                                                           
************************************************************************/
void CActorSprite2D::initPhysics()
{

}


/************************************************************************
*    DESC:  React to what the player is doing
************************************************************************/
void CActorSprite2D::handleEvent( const SDL_Event & rEvent )
{
    if( m_upAI )
        m_upAI->handleEvent( rEvent );

}   // HandleEvent


/************************************************************************
*    DESC:  Update the actor
************************************************************************/
void CActorSprite2D::update()
{
    if( m_upAI )
        m_upAI->update();

    for( auto & iter : m_spriteDeq )
        iter.update();

}   // Update


/************************************************************************
*    DESC:  Update the physics
************************************************************************/
void CActorSprite2D::physicsUpdate()
{

}   // PhysicsUpdate


/************************************************************************
*    DESC:  Transform the actor
************************************************************************/
void CActorSprite2D::transform()
{
    CObject2D::transform();

    for( auto & iter : m_spriteDeq )
        iter.transform( getMatrix(), wasWorldPosTranformed() );

}   // Transform

void CActorSprite2D::transform( const CObject2D & object )
{
    CObject2D::transform( object.getMatrix(), object.wasWorldPosTranformed() );

    for( auto & iter : m_spriteDeq )
        iter.transform( getMatrix(), wasWorldPosTranformed() );

}   // Transform

void CActorSprite2D::transform( const CMatrix & matrix, bool tranformWorldPos )
{
    CObject2D::transform( matrix, tranformWorldPos );

    for( auto & iter : m_spriteDeq )
        iter.transform( getMatrix(), wasWorldPosTranformed() );

}   // Transform


/************************************************************************
*    DESC:  Render the actor
************************************************************************/
void CActorSprite2D::render( const CMatrix & matrix )
{
    // Render in reverse order
    if( inView() )
    {
        for( auto & iter : m_spriteDeq )
            iter.render( matrix );
    }

}   // Render


/************************************************************************
*    DESC:  Get the physics component                                                            
************************************************************************/
CPhysicsComponent2D & CActorSprite2D::getPhysicsComponent()
{
    return m_physicsComponent;
    
}   // GetPhysicsComponent


/************************************************************************
*    DESC:  Get the sprite
************************************************************************/
CSprite2D & CActorSprite2D::getSprite( int index )
{
    if( index < 0 )
        return m_spriteDeq.back();
    else
        return m_spriteDeq.at(index);

}   // GetSprite


/************************************************************************
*    DESC:  Get the sprite group
************************************************************************/
CSprite2D & CActorSprite2D::getSprite( const std::string & name )
{
    auto iter = m_pSpriteMap.find( name );
    if( iter == m_pSpriteMap.end() )
        throw NExcept::CCriticalException( "Actor Sprite Access Error!",
            boost::str( boost::format("Sprite name does not exist (%s).\n\n%s\nLine: %s")
                % name % __FUNCTION__ % __LINE__ ) );

    return *iter->second;

}   // GetSprite


/************************************************************************
*    DESC:  Render the actor
************************************************************************/
bool CActorSprite2D::inView()
{
    if( m_projectionType == NDefs::EPT_ORTHOGRAPHIC )
        return inOrthographicView();

    else if( m_projectionType == NDefs::EPT_PERSPECTIVE )
        return inPerspectiveView();

    return true;

}   // InView


/************************************************************************
 *    DESC:  Check if an object is within the orthographic view frustum
 ************************************************************************/
bool CActorSprite2D::inOrthographicView()
{
    const CSize<float> & defaultSizeHalf = CSettings::Instance().getDefaultSizeHalf();
    
    // Check against the right side of the screen
    if( std::fabs(m_transPos.x) > (defaultSizeHalf.w + m_scaledRadius) )
        return false;

    // Check against the top of the screen
    if( std::fabs(m_transPos.y) > (defaultSizeHalf.h + m_scaledRadius) )
        return false;

    // If we made it this far, the object is in view
    return true;

}   // InOrthographicView


/************************************************************************
 *    DESC:  Check if an object is within the perspective view frustum
 ************************************************************************/
bool CActorSprite2D::inPerspectiveView()
{
    const CSize<float> & aspectRatio = CSettings::Instance().getScreenAspectRatio();

    // Check the right and left sides of the screen
    if( std::fabs(m_transPos.x) > ((std::fabs(m_pos.z) * aspectRatio.w) + m_scaledRadius) )
        return false;

    // Check the top and bottom sides of the screen
    if( std::fabs(m_transPos.y) > ((std::fabs(m_pos.z) * aspectRatio.h) + m_scaledRadius) )
        return false;

    // if we made it this far, the object was not culled
    return true;

}   // InPerspectiveView


/************************************************************************
*    DESC:  Apply the scale to the radius
************************************************************************/
void CActorSprite2D::applyScale( CMatrix & matrix )
{
    CObject2D::applyScale( matrix );

    // Find the largest actor scale
    float scale = getScale().x;
    if( getScale().y > scale )
        scale = getScale().y;

    m_scaledRadius = m_radius * scale;
    
    // Init the radius for collision
    m_collisionRadius = m_scaledRadius * m_collisionRadiusScalar;

}   // ApplyScale


/************************************************************************
*    DESC:  Get the collision group
************************************************************************/
uint CActorSprite2D::getCollisionGroup() const
{
    return m_collisionGroup;
}


/************************************************************************
*    DESC:  Get the collision radius
************************************************************************/
float CActorSprite2D::getCollisionRadius() const
{
    return m_collisionRadius;
}


/***************************************************************************
*    DESC:  Check for broad phase collision against other actor sprite
****************************************************************************/
bool CActorSprite2D::isCollision( CActorSprite2D & rActor )
{
    bool result(false);
    
    if( m_collisionMask & rActor.getCollisionGroup() )
    {
        result = checkBroadPhase( rActor );
    }
    
    return result;
    
}   // IsCollision


/***************************************************************************
*    DESC:  Check for broad phase collision against other actor sprite
****************************************************************************/
bool CActorSprite2D::checkBroadPhase( CActorSprite2D & rActor )
{
    const float radius = m_collisionRadius + rActor.getCollisionRadius();
    const float length = m_transPos.getLength2D( rActor.getTransPos() );
    
    return (length < radius);
    
}   // CheckBroadPhase
