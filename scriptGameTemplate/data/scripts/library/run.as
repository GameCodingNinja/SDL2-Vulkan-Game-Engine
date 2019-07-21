
//
//  FILE NAME:  run.as
//
//  DESC:       Scripts specifically for the run state
//


//
//  AI Init script
//
void Run_Init( CSprite & sprite )
{
    InitPhysics( sprite );
}


//
//  AI Update script
//
void Run_Update( CSprite & sprite )
{
    while( true )
    {
        if( sprite.getPos().y > 600.f )
            InitPhysics( sprite );
        Suspend();
    }
}

//
//  Init the ball placement
//
void InitPhysics( CSprite & sprite )
{
    sprite.setPhysicsTransform( RandInt(-700,700), -RandInt(600,1000), RandInt(0,360) );
}