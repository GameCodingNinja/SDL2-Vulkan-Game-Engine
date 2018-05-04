
/************************************************************************
 *    FILE NAME:       titlescreenstate.cpp
 *
 *    DESCRIPTION:     Class template
 ************************************************************************/

// Physical component dependency
#include "titlescreenstate.h"

// Game lib dependencies
#include <utilities/settings.h>
#include <utilities/deletefuncs.h>
#include <utilities/settings.h>
#include <utilities/genfunc.h>
#include <utilities/exceptionhandling.h>
#include <utilities/highresolutiontimer.h>
#include <objectdata/objectdatamanager.h>
#include <system/device.h>
#include <gui/menumanager.h>
#include <gui/menu.h>
#include <gui/uislider.h>
#include <gui/uibuttonlist.h>
#include <managers/actionmanager.h>
#include <managers/signalmanager.h>
#include <common/defs.h>
#include <common/camera.h>
#include <script/scriptcomponent.h>
#include <physics/physicsworldmanager.h>
#include <physics/physicsworld3d.h>

// Boost lib dependencies
#include <boost/format.hpp>

// Standard lib dependencies
#include <chrono>

// SDL lib dependencies
#include <SDL.h>

/************************************************************************
 *    desc:  Constructer                                                             
 ************************************************************************/
CTitleScreenState::CTitleScreenState() :
    m_background( CObjectDataMgr::Instance().GetData2D( "(title_screen)", "background" ) ),
    m_SpriteIdInc(0),
    m_allowStateChange(false),
    m_physicsWorld( CPhysicsWorldManager::Instance().GetWorld3D( "(cube)" ) ),
    m_consecutiveMax(0)
{
    // Set the game states for this object
    m_gameState = EGS_TITLE_SCREEN;
    m_nextState = EGS_GAME_LOAD;

    // Clear out any active trees and set the active one and activate the root menu
    CMenuManager::Instance().ClearActiveTrees();
    CMenuManager::Instance().ActivateTree("title_screen");

    // Setup to handle sprite signal
    CSignalMgr::Instance().Connect_Sprite( boost::bind(&CTitleScreenState::HandleSpriteSignal, this, _1, _2, _3) );

    // Create the plane on the left of the screen
    m_upLeftPlane.reset( new CSprite3D( CObjectDataMgr::Instance().GetData3D( "(cube)", "plane" ) ) );
    m_upLeftPlane->SetPos( CPoint<float>(-70.f, 0.f, 0.f) );
    m_upLeftPlane->SetRot( CPoint<float>(0.f, 0.f, -90.f) );
    m_upLeftPlane->InitPhysics();

    // Create the plane on the right of the screen
    m_upRightPlane.reset( new CSprite3D( CObjectDataMgr::Instance().GetData3D( "(cube)", "plane" ) ) );
    m_upRightPlane->SetPos( CPoint<float>(70.f, 0.f, 0.f) );
    m_upRightPlane->SetRot( CPoint<float>(0.f, 0.f, 90.f) );
    m_upRightPlane->InitPhysics();

    // Create the plane on the right of the screen
    m_upFrontPlane.reset( new CSprite3D( CObjectDataMgr::Instance().GetData3D( "(cube)", "plane" ) ) );
    m_upFrontPlane->SetPos( CPoint<float>(0.f, 0.f, -50.f) );
    m_upFrontPlane->SetRot( CPoint<float>(-90.f, 0.f, 0.f) );
    m_upFrontPlane->InitPhysics();

    // Seed the random number generator
    auto timeInNano = std::chrono::high_resolution_clock::now().time_since_epoch();
    m_generator.seed( std::chrono::duration_cast<std::chrono::nanoseconds>(timeInNano).count() );
    
    CCamera::Instance().SetPos( CPoint<float>(0,0,-20) );
    CCamera::Instance().SetRot( CPoint<float>(20,0,0) );

}   // Constructer

/************************************************************************
 *    desc:  destructer
 ************************************************************************/
CTitleScreenState::~CTitleScreenState()
{
    NDelFunc::DeleteMapPointers( pSpriteMap );
    NDelFunc::DeleteVectorPointers( pSpriteVec );

}   // destructer

/************************************************************************
 *    desc:  Handle events
 ************************************************************************/
void CTitleScreenState::HandleEvent( const SDL_Event & rEvent )
{
    CCommonState::HandleEvent( rEvent );

    if( CActionMgr::Instance().WasAction( rEvent, "Shoot", NDefs::EAP_DOWN ) &&
            !CMenuManager::Instance().IsMenuItemActive() )
    {
        // Get the consecutive explosion count
        m_consecutiveMax =
            CMenuManager::Instance().GetMenuControl<CUISlider>(
            "title_screen_menu", "consecutive_explosions_slider" ).GetValue();

        CSprite3D * pSprite = new CSprite3D( CObjectDataMgr::Instance().GetData3D( "(cube)", "cubeShoot" ) );
        pSpriteMap.emplace( ++m_SpriteIdInc, pSprite );

        if( rEvent.type == SDL_MOUSEBUTTONDOWN )
        {
            float x(0), y(0), posZ( 150 );

            auto timeInNano = std::chrono::high_resolution_clock::now().time_since_epoch();
            colorSeed = std::chrono::duration_cast<std::chrono::nanoseconds>(timeInNano).count();

            // Convert 2d screen coordinates to 3D perspective space
            CSize<float> scrnSize = CSettings::Instance().GetSize();
            NGenFunc::Convert2Dto3D( x, y, rEvent.button.x, rEvent.button.y, scrnSize.w, scrnSize.h );
            std::uniform_int_distribution<int> randPosX( -50, 50 );
            CPoint<float> startPos( randPosX( m_generator ), -70, -posZ );
            CPoint<float> endPos( x * posZ, y * posZ, -posZ );

            pSprite->SetPos( startPos );
            pSprite->SetScale( CPoint<float>( 2, 2, 2 ) );
            pSprite->SetColor( RandomizeColor( colorSeed ) );
            pSprite->GetScriptComponent().Prepare( "ShootTo",{2000.f, &endPos, pSprite, m_SpriteIdInc } );

            timeInNano = std::chrono::high_resolution_clock::now().time_since_epoch();
            colorSeed = std::chrono::duration_cast<std::chrono::nanoseconds>(timeInNano).count();
        }
    }

}   // HandleEvent

/************************************************************************
 *    desc:  Handle sprite signals
 ************************************************************************/
void CTitleScreenState::HandleSpriteSignal( int msg, int code1, int code2 )
{
    if( msg == NDefs::ESM_KILL_SPRITE )
    {
        // Add the delete index
        spriteDeleteVec.push_back( code1 );

        // Create the box sprite explosion
        CreateExplosion( code1, code2 );
    }

}   // HandleSpriteSignal

/************************************************************************
 *    desc:  Create the box sprite explosion
 ************************************************************************/
void CTitleScreenState::CreateExplosion( const int spriteIndex, const int consecutiveCount )
{
    // Find the sprite to explode from
    const auto & iter = pSpriteMap.find( spriteIndex );
    if( iter == pSpriteMap.end() )
    {
        throw NExcept::CCriticalException( "Error Creating Physics Sprite!",
            boost::str( boost::format( "Shoot sprite index can't be found (%d).\n\n%s\nLine: %s" )
            % iter->first % __FUNCTION__ % __LINE__ ) );
    }

    // Get the explosion count
    const int explosionCount =
        CMenuManager::Instance().GetMenuControl<CUISlider>(
        "title_screen_menu", "cubes_explosion_slider" ).GetValue();

    // Get the restitution
    const float restitution =
        CMenuManager::Instance().GetMenuControl<CUIButtonList>(
        "title_screen_menu", "cubes_bounce_btn_lst" ).GetActiveIndex() * 0.5f;

    m_upLeftPlane->GetPhysicsComponent().SetRestitution(restitution);
    m_upRightPlane->GetPhysicsComponent().SetRestitution(restitution);
    m_upFrontPlane->GetPhysicsComponent().SetRestitution(restitution);

    // Define the ranges of random numbers
    std::uniform_real_distribution<float> linVel( -1, 1 );
    std::uniform_real_distribution<float> linVelY( 0, 1 );
    std::uniform_int_distribution<int> avgVel( -5, 5 );

    CColor color = RandomizeColor( colorSeed * consecutiveCount * explosionCount );

    for( int i = 0; i < explosionCount; ++i )
    {
        CSprite3D * pSprite = new CSprite3D( CObjectDataMgr::Instance().GetData3D( "(cube)", "cubePhysics" ) );

        // Set the position and start the physics
        pSprite->SetPos( iter->second->GetPos() );
        pSprite->SetScale( iter->second->GetScale() );
        pSprite->SetColor( iter->second->GetColor() );

        pSprite->InitPhysics();

        pSprite->GetPhysicsComponent().SetRestitution(restitution);

        CPoint<float> vol( linVel( m_generator ), linVelY( m_generator ), linVel( m_generator ) );
        pSprite->GetPhysicsComponent().SetLinearVelocity(vol * (25 + (5 - m_consecutiveMax)));

        pSprite->GetPhysicsComponent().SetAngularVelocity(
            CRadian<float>( avgVel( m_generator ), avgVel( m_generator ), avgVel( m_generator ) ) );

        if( consecutiveCount < m_consecutiveMax )
        {
            pSpriteMap.emplace( ++m_SpriteIdInc, pSprite );
            pSprite->GetScriptComponent().Prepare( "ChangeColor",{ pSprite, &color, m_SpriteIdInc, consecutiveCount + 1 } );
        }
        else
        {
            pSpriteVec.push_back( pSprite );
            pSprite->GetScriptComponent().Prepare( "RandomColorTo",{ pSprite } );
        }
    }

}   // CreateExplosion

/***************************************************************************
 *    desc:  Generate a random color
 ****************************************************************************/
CColor CTitleScreenState::RandomizeColor( uint seed )
{
    CColor finalColor;

    std::default_random_engine generator( seed );
    std::uniform_int_distribution<int> randColor( 0, 5 );

    switch( randColor( generator ) )
    {
        case 0:
            finalColor.Set( 1, 0, 0, 1 );
            break;

        case 1:
            finalColor.Set( 0, 1, 0, 1 );
            break;

        case 2:
            finalColor.Set( 0, 0, 1, 1 );
            break;

        case 3:
            finalColor.Set( 1, 1, 0, 1 );
            break;

        case 4:
            finalColor.Set( 1, 0, 1, 1 );
            break;

        case 5:
            finalColor.Set( 1, 0, 1, 1 );
            break;
    }

    std::uniform_int_distribution<int> randInt( 0, 359 );
    finalColor.TransformHSV( randInt( generator ), 1, 1 );

    return finalColor;

}   // RandomizeColor

/***************************************************************************
 *    desc:  Handle the physics
 ****************************************************************************/
void CTitleScreenState::Physics()
{
    // Free any pending sprites
    FreeSprites();

    m_physicsWorld.Step();

}   // Physics

/***************************************************************************
 *    desc:  Update objects that require them
 ****************************************************************************/
void CTitleScreenState::Update()
{
    CCommonState::Update();

    for( auto iter : pSpriteMap )
        iter.second->Update();

    for( auto iter : pSpriteVec )
        iter->Update();

    m_background.Update();

}   // Update

/***************************************************************************
 *    desc:  Transform the game objects
 ****************************************************************************/
void CTitleScreenState::Transform()
{
    CCommonState::Transform();

    m_background.Transform();

    for( auto iter : pSpriteMap )
        iter.second->Transform();

    for( auto iter : pSpriteVec )
        iter->Transform();
    
    CCamera::Instance().Transform();

}   // Transform

/***************************************************************************
 *    desc:  2D/3D Render of game content
 ****************************************************************************/
void CTitleScreenState::PreRender()
{
    CCommonState::PreRender();

    glDisable( GL_DEPTH_TEST );

    m_background.Render( CDevice::Instance().GetProjectionMatrix( NDefs::EPT_ORTHOGRAPHIC ) );

    glEnable( GL_DEPTH_TEST );

    CMatrix dummyMatrix;
    const CMatrix matrix = CDevice::Instance().GetProjectionMatrix( NDefs::EPT_PERSPECTIVE );
    for( auto iter : pSpriteMap )
        iter.second->Render( matrix, dummyMatrix );

    for( auto iter : pSpriteVec )
        iter->Render( matrix, dummyMatrix );

}   // PreRender

/***************************************************************************
 *    desc:  Free the sprites in the queue
 ****************************************************************************/
void CTitleScreenState::FreeSprites()
{
    for( auto delIter : spriteDeleteVec )
    {
        // Find the sprite, delete it and remove entry from map
        const auto & iter = pSpriteMap.find( delIter );
        if( iter != pSpriteMap.end() )
        {
            NDelFunc::Delete( iter->second );
            pSpriteMap.erase( iter );
        }
        else
        {
            throw NExcept::CCriticalException("Error Deleting Sprite!",
                boost::str( boost::format("Sprite index can't be found (%d).\n\n%s\nLine: %s")
                % iter->first % __FUNCTION__ % __LINE__ ));
        }
    }

    spriteDeleteVec.clear();

    // If the sprite has left the view fustrum from the bottom, delete it.
    const float scaler = CSettings::Instance().GetScreenAspectRatio().h;
    for( auto iter = pSpriteVec.begin(); iter != pSpriteVec.end(); )
    {
        const CPoint<float> pos = (*iter)->GetPos();
        if( fabs(pos.y) > fabs(pos.z) * scaler )
        {
            NDelFunc::Delete(*iter);
            iter = pSpriteVec.erase( iter );
        }
        else
            ++iter;
    }

}	// FreeSprites

