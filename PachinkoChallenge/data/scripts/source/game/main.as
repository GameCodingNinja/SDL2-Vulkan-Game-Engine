
//
//  FILE NAME:  main.as
//
//  DESC:       Main game function
//

void main()
{
    // Create the game
    CGame game;
    game.init();

    // Call the game loop
    while( game.gameLoop() )
        Suspend();
    
    // Do the cleanup
    game.destroy();
}
