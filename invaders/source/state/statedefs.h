
/************************************************************************
*    FILE NAME:       statedefs.h
*
*    DESCRIPTION:     These defs are custom per game project
************************************************************************/

#pragma once

namespace NStateDefs
{
    // These states are custom per game project
    enum EGameState
    {
        EGS_NULL,
        EGS_STARTUP,
        EGS_TITLE_SCREEN,
        EGS_GAME_LOAD,
        EGS_LEVEL_1
    };
    
    enum EStateEvent
    {
        ESE_USER_EVENTS = 0x9000,
        ESE_FADE_IN_COMPLETE,
        ESE_FADE_OUT_COMPLETE,
        ESE_THREAD_LOAD_COMPLETE,
    };
}
