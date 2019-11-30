
//
//  FILE NAME:  main.as
//
//  DESC:       Main game function
//

CGame @hGame = null;

void main()
{
    // Create the game
    CGame game;
    game.init();
    @hGame = game;

    // Call the game loop
    while( game.gameLoop() )
        {}
    
    // Do the cleanup
    @hGame = null;
    game.destroy();
}

/************************************************************************
*    DESC:  Physics callbacks
************************************************************************/
void Physics_BeginContact( CSprite & spriteA, CSprite & spriteB )
{
    if( hGame !is null )
        hGame.beginContact( spriteA, spriteB );
}

void Physics_EndContact( CSprite & spriteA, CSprite & spriteB )
{
    if( hGame !is null )
        hGame.endContact( spriteA, spriteB );
}

void Physics_DestroyFixture( CSprite & sprite )
{
    if( hGame !is null )
        hGame.destroyFixture( sprite );
}

void Physics_DestroyJoint( CSprite & sprite )
{
    if( hGame !is null )
        hGame.destroyJoint( sprite );
}