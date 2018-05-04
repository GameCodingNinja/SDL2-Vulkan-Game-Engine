
/************************************************************************
*    FILE NAME:       runstate.cpp
*
*    DESCRIPTION:     CRunState Class State
************************************************************************/

// Physical component dependency
#include "runstate.h"

// Game lib dependencies
#include <gui/menumanager.h>
#include <utilities/highresolutiontimer.h>
#include <utilities/statcounter.h>
#include <objectdata/objectdata2d.h>
#include <objectdata/objectdatamanager.h>
#include <system/device.h>
#include <physics/physicsworldmanager.h>
#include <physics/physicsworld2d.h>

/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CRunState::CRunState() :
    m_changeState(false),
    m_box( CObjectDataMgr::Instance().GetData2D( "(run)", "box" ) ),
    m_triangle( CObjectDataMgr::Instance().GetData2D( "(run)", "triangle" ) ),
    m_circle( CObjectDataMgr::Instance().GetData2D( "(run)", "circle" ) ),
    m_floor( CObjectDataMgr::Instance().GetData2D( "(run)", "floor" ) ),
    m_physicsWorld( CPhysicsWorldManager::Instance().GetWorld2D( "(game)" ) )
{
    // Set the game state for this object
    m_gameState = EGS_RUN;

    // Clear out any active trees and set the active one and activate the root menu
    CMenuManager::Instance().ClearActiveTrees();
    CMenuManager::Instance().ActivateTree("pause");
    
    m_floor.SetPos( CPoint<float>(0,-400,0) );
    //m_floor.SetRot( CPoint<float>(0,0,15) );
    m_floor.InitPhysics();
    
    m_box.SetPos( CPoint<float>(50,600,0) );
    m_box.SetRot( CPoint<float>(0,0,25) );
    //m_box.SetScale( CPoint<float>(3,3,3) );
    m_box.InitPhysics();
    
    m_triangle.SetPos( CPoint<float>(0,500,0) );
    m_triangle.SetRot( CPoint<float>(0,0,25) );
    //m_triangle.SetScale( CPoint<float>(3,3,3) );
    m_triangle.InitPhysics();
    
    m_circle.SetPos( CPoint<float>(-50,700,0) );
    //m_circle.SetScale( CPoint<float>(3,3,3) );
    m_circle.InitPhysics();

}   // Constructer


/************************************************************************
*    desc:  Handle events
************************************************************************/
void CRunState::HandleEvent( const SDL_Event & rEvent )
{
    CCommonState::HandleEvent( rEvent );

    // Check for the "change state" message
    if( rEvent.type == NMenu::EGE_MENU_BACK_TO_MAIN_MENU )
    {
        m_stateMessage.Clear();
        m_stateMessage.m_nextState = EGS_TITLE_SCREEN;
        m_stateMessage.m_lastState = EGS_RUN;

        // Load the object data
        m_stateMessage.m_groupLoad.push_back("(title_screen)");
        
        // Unload the object data
        m_stateMessage.m_groupUnload.push_back("(run)");
        
        // Physics world to destroy
        m_stateMessage.m_physicsWorldDestroy = "(game)";

        m_nextState = EGS_GAME_LOAD;

        m_changeState = true;
    }

}   // HandleEvent


/***************************************************************************
*    desc:  Check for collision and react to it
****************************************************************************/
bool CRunState::DoStateChange()
{
    return m_changeState;

}   // DoStateChange


/***************************************************************************
*    desc:  Handle the physics
****************************************************************************/
void CRunState::Physics()
{
    if( !CMenuManager::Instance().IsActive() )
    {
        m_physicsWorld.Step();
    }

}   // Physics


/***************************************************************************
*    desc:  Update objects that require them
****************************************************************************/
void CRunState::Update()
{
    CCommonState::Update();
    
    if( !CMenuManager::Instance().IsActive() )
    {
        m_box.Update();
        m_triangle.Update();
        m_circle.Update();

        // If they fall off the floor, reposition to the top again
        if( m_box.GetPos().y < -600.0f )
            m_box.GetPhysicsComponent().SetTransform( 50, 600, 25 );

        if( m_triangle.GetPos().y < -600.0f )
            m_triangle.GetPhysicsComponent().SetTransform( 0, 600, 25 );

        if( m_circle.GetPos().y < -600.0f )
            m_circle.GetPhysicsComponent().SetTransform( -50, 600, 0 );
    }

}   // Update


/***************************************************************************
*    desc:  Transform the game objects
****************************************************************************/
void CRunState::Transform()
{
    CCommonState::Transform();
    
    if( !CMenuManager::Instance().IsActive() )
    {
        m_box.Transform();
        m_triangle.Transform();
        m_circle.Transform();
        m_floor.Transform();
    }

}   // Transform


/***************************************************************************
*    desc:  Do the 2D rendering
****************************************************************************/
void CRunState::PreRender()
{
    CCommonState::PreRender();
    
    const CMatrix & matrix = CDevice::Instance().GetProjectionMatrix( NDefs::EPT_ORTHOGRAPHIC );
    m_box.Render( matrix );
    m_triangle.Render( matrix );
    m_circle.Render( matrix );
    m_floor.Render( matrix );

}   // PreRender

