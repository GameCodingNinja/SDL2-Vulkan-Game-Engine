
/************************************************************************
*    FILE NAME:       level1.as
*
*    DESCRIPTION:     Scripts for level 1
************************************************************************/

/************************************************************************
*    DESC:  ball AI
************************************************************************/
void BallAI( CSprite & sprite )
{
    // Serves at the sprite init
    sprite.setPhysicsTransform( RandInt(-700,700), -RandInt(600,1000), RandInt(0,360) );

    Suspend();

    do
    {
        if( sprite.getPos().y > 600.f )
            sprite.setPhysicsTransform( RandInt(-700,700), -RandInt(600,1000), RandInt(0,360) );

        Suspend();
    }
    while(true);
}
