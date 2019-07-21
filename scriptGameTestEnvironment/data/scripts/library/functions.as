
/************************************************************************
*    FILE NAME:       functions.as
*
*    DESCRIPTION:     General purpose scripts
************************************************************************/

/************************************************************************
*    DESC:  Looping Animation
************************************************************************/
void loopAnimation( CSprite & sprite )
{
    NSpriteUtils::Play( 18, sprite, true );
}

/************************************************************************
*    DESC:  Rotate the gun
************************************************************************/
void rotateGun( CSprite & sprite )
{
    float gunRotation = 0.f;
    
    while( true )
    {
        if( !ActionMgr.queueEmpty() )
        {
            if( ActionMgr.wasLastDeviceMouse() )
            {
                // A game resolution change will change these  
                // values so we need to get them each time
                float ratio = 1 / Settings.getOrthoAspectRatio().h;
                CSize halfSize = Settings.getSizeHalf();

                CPoint spritePos = sprite.getWorldPos();
                CPoint mousePos = ActionMgr.getMouseAbsolutePos();
                gunRotation = -(atan2( (ratio * (halfSize.w - mousePos.x)) + spritePos.x, (ratio * (halfSize.h - mousePos.y)) + spritePos.y ) + M_PI_2);
                sprite.setRot( 0, 0, gunRotation, false );
            }
            else if( ActionMgr.wasLastDeviceGamepad() )
            {
                CPoint pos = ActionMgr.getControllerPosLeft();
                gunRotation = (atan2( -pos.x, pos.y ) + M_PI_2);
                sprite.setRot( 0, 0, gunRotation, false );
            }
            
            if( ActionMgr.wasAction("Shoot") )
            {
                CSprite @ projectile = StrategyMgr.getStrategy( "(main)" ).create( "projectile" ).getSprite();
                
                CPoint pos = sprite.getWorldPos();
                CPoint centerPos = sprite.getCenterPos();
                const float OFFSET = 50.f;
                CPoint projectileOffset;
                projectileOffset.x = (OFFSET * cos( gunRotation )) + pos.x;
                projectileOffset.y = (OFFSET * sin( gunRotation )) + pos.y;
                
                projectile.setPos( projectileOffset );
                projectile.setRot( 0, 0, gunRotation, false );
            }
        }
        
        Suspend();
    }
}


/************************************************************************
*    DESC:  Move Projectile
************************************************************************/
void MoveProjectile( CSprite & sprite )
{
    // Speed of the projectile
    const float PROJECTILE_SPEED = 1.5f;

    // Set the velocity of the projectile
    float rotation = sprite.getRot().z;
    CPoint velocity;
    velocity.x = cos( rotation ) * PROJECTILE_SPEED;
    velocity.y = sin( rotation ) * PROJECTILE_SPEED;
    
    // Hold on to the stating position to calculate length
    CPoint startPos = sprite.getWorldPos();

    while( true )
    {
        //float time = HighResTimer.getElapsedTime();
        sprite.incPos( velocity * HighResTimer.getElapsedTime() );
        
        // Delete if goes out of view
        if( sprite.getWorldPos().getLengthSquared2D() > 1300000.f )
        {
            StrategyMgr.getStrategy( "(main)" ).destroy( sprite.getId() );
            break;
        }
        
        Suspend();
    }
}