
//
//  FILE NAME:  level1.as
//
//  DESC:       Scripts specifically for the level 1 state
//


//
//  AI Update script
//
void Level1_BallAI( CSprite & sprite )
{
    sprite.setPhysicsTransform( RandInt(-700,700), -RandInt(600,1000), RandInt(0,360) );

    Suspend();

    while( true )
    {
        if( sprite.getPos().y > 600.f )
            sprite.setPhysicsTransform( RandInt(-700,700), -RandInt(600,1000), RandInt(0,360) );

        Suspend();
    }
}

//
//  AI Update script
//
void Level1_PegOff( CSprite & sprite )
{
    Hold( 200 );

    sprite.setFrame(0);
}