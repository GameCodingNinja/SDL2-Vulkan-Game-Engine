
/************************************************************************
 *    FILE NAME:       sprite3d.cpp
 *
 *    DESCRIPTION:     3D sprite class
 ************************************************************************/

// Physical component dependency
#include <3d/sprite3d.h>

// Game lib dependencies
#include <objectdata/objectdata3d.h>
#include <objectdata/objectphysicsdata3d.h>
#include <utilities/exceptionhandling.h>
#include <common/color.h>
#include <common/camera.h>
#include <common/iaibase.h>

// Bullet Physics lib dependencies
#include <btBulletCollisionCommon.h>

/************************************************************************
 *    DESC:  Constructor
 ************************************************************************/
CSprite3D::CSprite3D( const CObjectData3D & objectData, int id ) :
    iSprite(id),
    m_objectData(objectData),
    m_visualComponent(objectData.getVisualData()),
    m_physicsComponent(objectData.getPhysicsData())
{
    // If there's no visual data, set the hide flag
    setVisible( objectData.getVisualData().isActive() );
}


/************************************************************************
 *    DESC:  destructor                                                             
 ************************************************************************/
CSprite3D::~CSprite3D()
{
}


/************************************************************************
 *    DESC:  Init the physics                                                           
 ************************************************************************/
void CSprite3D::initPhysics()
{
    m_physicsComponent.init( *this );
}


/************************************************************************
*    DESC:  Set the translation/rotation from Bullet Physics
************************************************************************/
void CSprite3D::setTransform( const btTransform & trans )
{
    m_parameters.add( NDefs::ROTATE | NDefs::PHYSICS_TRANSFORM );
    
    // Set the position
    const btVector3 & btVec = trans.getOrigin();
    setPos( btVec.x(), btVec.y(), btVec.z() );
    
    // Set the rotation
    const btMatrix3x3 & btMat = trans.getBasis();
    for( int i = 0; i < 3; ++i )
    {
        const btVector3 & vec = btMat.getRow(i);
        m_rotMatrix.setColumn( i, vec.x(), vec.y(), vec.z() );
    }
    
    // This is an example of how to get the rotation out of bullet physics
    // but it's a lot of extra work to only do the rotation calculation all over again.
    
    // Get the rotation
    //const btMatrix3x3 & btMat = trans.getBasis();
    //btScalar z, y, x;
    //btMat.getEulerYPR( z, y, x );
    
    // Set the rotation
    //SetRotXYZ( x, y, z, false );
}


/************************************************************************
*    DESC:  React to what the player is doing
************************************************************************/
void CSprite3D::handleEvent( const SDL_Event & rEvent )
{
    if( m_upAI )
        m_upAI->handleEvent( rEvent );
}


/************************************************************************
 *    DESC:  Update the sprite                                                           
 ************************************************************************/
void CSprite3D::update()
{
    if( isVisible() )
        m_physicsComponent.update( this );

    m_scriptComponent.update();

    if( m_upAI )
        m_upAI->update();
}


/************************************************************************
*    DESC:  Update the physics
************************************************************************/
void CSprite3D::physicsUpdate()
{
    m_physicsComponent.update( this );
}


/************************************************************************
 *    DESC:  do the render                                                            
 ************************************************************************/
void CSprite3D::render( const CMatrix & matrix, const CMatrix & rotMatrix )
{
    if( isVisible() )
        m_visualComponent.render( m_matrix * matrix, m_rotMatrix * rotMatrix );
}

void CSprite3D::render( const CCamera & camera )
{
    if( isVisible() )
        m_visualComponent.render( m_matrix * camera.getFinalMatrix(), m_rotMatrix * camera.getRotMatrix() );
}


/************************************************************************
 *    DESC:  Get the visual component                                                            
 ************************************************************************/
CVisualComponent3D & CSprite3D::getVisualComponent()
{
    return m_visualComponent;
}

iVisualComponent * CSprite3D::getVisualInterface()
{
    return &m_visualComponent;
}


/************************************************************************
 *    DESC:  Get the physics component                                                            
 ************************************************************************/
CPhysicsComponent3D & CSprite3D::getPhysicsComponent()
{
    return m_physicsComponent;
}

iPhysicsComponent * CSprite3D::getPhysicsInterface()
{
    return &m_physicsComponent;
}


/************************************************************************
 *    DESC:  Get the scripting component                                                            
 ************************************************************************/
CScriptComponent & CSprite3D::getScriptComponent()
{
    return m_scriptComponent;
}


/************************************************************************
 *    DESC:  Get the object data                                                            
 ************************************************************************/
const CObjectData3D & CSprite3D::getObjectData() const
{
    return m_objectData;
}


/************************************************************************
*    DESC:  Set/Get the AI pointer. This class owns the pointer
************************************************************************/
void CSprite3D::setAI( iAIBase * pAIBase )
{
    m_upAI.reset( pAIBase );

    // Handle any initialization in a separate function
    m_upAI->init();
}
