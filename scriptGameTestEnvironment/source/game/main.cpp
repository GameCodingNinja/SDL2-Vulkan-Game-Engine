
/************************************************************************
*    FILE NAME:       main.cpp
*
*    DESCRIPTION:     main function
************************************************************************/

// Game dependencies
#include "game.h"

// Game lib dependencies
#include <script/scriptmanager.h>
#include <utilities/exceptionhandling.h>

// Standard lib dependencies
#include <string>
#include <ctime>

// SDL lib dependencies
#include <SDL3/SDL.h> // This needs to be here otherwise Android build will not run

int main( int argc, char* args[] )
{
    std::srand ( unsigned ( std::time(0) ) );

    CGame game;

    try
    {
        // Create the game
        game.create();

        // Call the game loop
        while( CScriptMgr::Instance().update() )
        { }
    }
    catch( NExcept::CCriticalException & ex )
    {
        game.displayErrorMsg( ex.getErrorTitle(), ex.getErrorMsg() );
    }
    catch( std::exception const & ex )
    {
        game.displayErrorMsg( "Standard Exception", ex.what() );
    }
    catch(...)
    {
        game.displayErrorMsg( "Unknown Error", "Something bad happened and I'm not sure what it was." );
    }

    return 0;
}
