
/************************************************************************
*    FILE NAME:       menu.as
*
*    DESCRIPTION:     Scripts for controlling/animating menu interface
************************************************************************/

/************************************************************************
*    DESC:  Menu transition in script
************************************************************************/
void Menu_TransIn( CMenu & menu )
{
    menu.setAlpha( 0.f );
    menu.setVisible( true );
    
    AlphaTo( 250, 1.f, menu );
    
    DispatchEvent( NMenuEvent::TRANS_IN, NTransCode::END );
}


/************************************************************************
*    DESC:  Menu transition out script
************************************************************************/
void Menu_TransOut( CMenu & menu )
{
    menu.setAlpha( 1.f );
    
    AlphaTo( 250, 0.f, menu );
    
    menu.setVisible( false );
    
    DispatchEvent( NMenuEvent::TRANS_OUT, NTransCode::END );
}


/************************************************************************
*    DESC:  Menu function to fade the alpha
************************************************************************/
void AlphaTo( float time, float final, CMenu & menu )
{
    float current = menu.getAlpha();
    float inc = (final - current) / time;

    do
    {
        time -= HighResTimer.getElapsedTime();

        if( time < 0 )
        {
            menu.setAlpha( final );
        }
        else
        {
            current += inc * HighResTimer.getElapsedTime();
            menu.setAlpha( current );
        }

        Suspend();
    }
    while( time > 0 );
}


/************************************************************************
*    DESC:  Sound to play when the menu control becomes active
************************************************************************/
void Control_OnActive( uiControl & control )
{
    if( !ActionMgr.wasLastDeviceMouse() )
        SoundMgr.play( "(menu)", "active" );
}


/************************************************************************
*    DESC:  Sound to play when the menu control is selected
************************************************************************/
void Control_OnSelect( uiControl & control )
{
    SoundMgr.play( "(menu)", "select" );
}


/************************************************************************
*    DESC:  Script for setting the look of the disabled state
************************************************************************/
void Control_Disabled( CSprite & sprite )
{
    CColor color = sprite.getDefaultColor();
    color.transformHSV( 0, 0, 1 );

    sprite.setColor( color );
}


/************************************************************************
*    DESC:  Script for setting the look of the inactive state
************************************************************************/
void Control_Inactive( CSprite & sprite )
{
    sprite.setDefaultColor();
}


/************************************************************************
*    DESC:  Script for setting the look of the hidden state
************************************************************************/
void Control_Hidden( CSprite & sprite )
{
    sprite.setVisible( false );
}


/************************************************************************
*    DESC:  Script for animating the look of the active state
************************************************************************/
void Ctrl_Active( CSprite & sprite, float hiHSV, float lowHSV )
{
    sprite.setVisible( true );

    CColor hiColor = sprite.getDefaultColor();
    hiColor.transformHSV( 0, 1, hiHSV );

    CColor lowColor = sprite.getDefaultColor();
    lowColor.transformHSV( 0, 1, lowHSV );

    while(true)
    {
        NSpriteUtils::ColorTo( 500, hiColor, sprite );
        NSpriteUtils::ColorTo( 500, lowColor, sprite );
    }
}

void Control_Active( CSprite & sprite )
{
    Ctrl_Active( sprite, 1.3, .5 );
}

void Control_Solid_Active( CSprite & sprite )
{
    Ctrl_Active( sprite, 1.1, .5 );
}


/************************************************************************
*    DESC:  Script for animating the look of the selected state
************************************************************************/
void Ctrl_Selected( CSprite & sprite, float hiHSV, float lowHSV )
{
    sprite.setVisible( true );

    CColor hiColor = sprite.getDefaultColor();
    hiColor.transformHSV( 0, 1, hiHSV );

    CColor lowColor = sprite.getDefaultColor();
    lowColor.transformHSV( 0, 1, lowHSV );

    sprite.setColor( hiColor );

    NSpriteUtils::ColorTo( 120, lowColor, sprite );
    NSpriteUtils::ColorTo( 100, hiColor, sprite );
}


/************************************************************************
*    DESC:  Animates the selected state and sends a message
************************************************************************/
void Control_Selected_Dispatch_Exe( CSprite & sprite )
{
    Ctrl_Selected( sprite, 1.7, .6 );

    sprite.setColor( sprite.getDefaultColor() );

    DispatchEvent( NMenuEvent::SELECT_EXECUTE );
}

void Control_Selected_Dispatch_Exe_Act( CSprite & sprite )
{
    Ctrl_Selected( sprite, 1.7, .6 );

    sprite.setColor( sprite.getDefaultColor() );

    DispatchEvent( NMenuEvent::SELECT_EXECUTE );
    DispatchEvent( NMenuEvent::REACTIVATE );
}

void Control_Selected_Visible( CSprite & sprite )
{
    Ctrl_Selected( sprite, 1.7, .6 );

    sprite.setColor( sprite.getDefaultColor() );
}


/************************************************************************
*    DESC:  Animates the selected state for solid
************************************************************************/
void Control_Selected( CSprite & sprite )
{
    Ctrl_Selected( sprite, 1.7, .6 );

    sprite.setVisible( false );
}

void Control_Selected_frame_visible( CSprite & sprite )
{
    Ctrl_Selected( sprite, 1.7, .6 );

    sprite.setDefaultColor();
}

void Control_Solid_Selected( CSprite & sprite )
{
    Ctrl_Selected( sprite, 1.5, .6 );

    sprite.setVisible( false );
}


/************************************************************************
*    DESC:  Meter on start
************************************************************************/
void Meter_OnStart( CSprite & sprite )
{
    sprite.setVisible( true );
    NSpriteUtils::AlphaTo( 300, 1, sprite );
}


/************************************************************************
*    DESC:  Meter on stop
************************************************************************/
void Meter_OnStop( CSprite & sprite )
{
}


/************************************************************************
*    DESC:  Meter on clear
************************************************************************/
void Meter_OnClear( CSprite & sprite, uint value )
{
    if( sprite.isVisible() )
    {
        NSpriteUtils::AlphaTo( 250, 0, sprite );
        sprite.setVisible( false );
        sprite.createFontString( "" + value );
    }
}
