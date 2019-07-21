/************************************************************************
*    FILE NAME:       state_defs.as
*
*    DESCRIPTION:     State defines
************************************************************************/

namespace NStateDefs
{
    shared enum EGameState
    {
        EGS_NULL,
        EGS_STARTUP,
        EGS_TITLE_SCREEN,
        EGS_GAME_LOAD,
        EGS_RUN
    }
    
    shared enum EStateEvent
    {
        ESE_USER_EVENTS = 0x9000,
        ESE_FADE_IN_COMPLETE,
        ESE_FADE_OUT_COMPLETE,
        ESE_THREAD_LOAD_COMPLETE,
    }
}
