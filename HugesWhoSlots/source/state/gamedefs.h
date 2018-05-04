
/************************************************************************
*    FILE NAME:       gamedefs.h
*
*    DESCRIPTION:     These defs are custom per game project
************************************************************************/

#ifndef __game_defs_h__
#define __game_defs_h__

namespace NGameDefs
{
    // These states are custom per game project
    enum EGameState
    {
        EGS_NULL,
        EGS_STARTUP,
        EGS_LOBBY,
        EGS_BIG_CASH_BACK,
        EGS_GAME_LOAD
    };
}

#endif  // __game_defs_h__


