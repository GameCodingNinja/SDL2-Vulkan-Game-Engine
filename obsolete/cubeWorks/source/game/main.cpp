
/************************************************************************
*    FILE NAME:       main.cpp
*
*    DESCRIPTION:     main class
************************************************************************/

// Standard lib dependencies
#include <memory>
#include <string>

// Game lib dependencies
#include <utilities/exceptionhandling.h>
#include <system/device.h>

// Game dependencies
#include "game.h"

int main( int argc, char* args[] )
{
    //SDL_Delay(7000);

    CGame game;

    try
    {
        // Create the game
        game.Create();

        // Call the game loop
        while( game.GameLoop() )
        { }
    }
    catch( NExcept::CCriticalException & ex )
    {
        game.DisplayErrorMsg( ex.GetErrorTitle(), ex.GetErrorMsg() );
    }
    catch( std::exception const & ex )
    {
        game.DisplayErrorMsg( "Standard Exception", ex.what() );
    }
    catch(...)
    {
        game.DisplayErrorMsg( "Unknown Error", "Something bad happened and I'm not sure what it was." );
    }

    return 0;

}   // main

