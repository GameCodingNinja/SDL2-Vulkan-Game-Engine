
/************************************************************************
*    FILE NAME:       state.as
*
*    DESCRIPTION:     Scripts for general state
************************************************************************/

/************************************************************************
*    DESC:  Do a fade in and out
************************************************************************/
void State_StartUpFadeIn()
{
    // This is initialized in startupstate.as
    CVisual visual;

    Hold( 500 );
            
    CColor final(1,1,1,1);
    NVisualUtils::ColorTo( 500, final, visual );
    
    DispatchEvent( NStateDefs::ESE_FADE_IN_COMPLETE );
}


void State_FadeIn()
{
    CVisual visual;
    visual.setAdditiveColor(0,0,0,1);
            
    CColor final(1,1,1,1);
    NVisualUtils::ColorTo( 500, final, visual );
    
    DispatchEvent( NStateDefs::ESE_FADE_IN_COMPLETE );
}

void State_FadeOut()
{
    CVisual visual;
    
    CColor current(0,0,0,1);
    NVisualUtils::ColorTo( 500, current, visual );
    
    DispatchEvent( NStateDefs::ESE_FADE_OUT_COMPLETE );
}


/************************************************************************
*    DESC:  Looping Animation
************************************************************************/
void State_LoopAnimation( CSprite & sprite )
{
    NSpriteUtils::Play( 12, sprite, true );
}


/************************************************************************
*    DESC:  Cube rotate
************************************************************************/
void State_RotateCube( CSprite & sprite )
{
    while( true )
    {
        float rot = HighResTimer.getElapsedTime() * 0.04;
        sprite.incRot( rot, rot, 0 );

        Suspend();
    }
}
