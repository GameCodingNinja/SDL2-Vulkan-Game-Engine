/************************************************************************
*    FILE NAME:       state_defs.as
*
*    DESCRIPTION:     State defines
************************************************************************/

namespace NStateDefs
{
    shared enum EStateEvent
    {
        ESE_USER_EVENTS = 0x9000,
        ESE_FADE_IN_COMPLETE,
        ESE_FADE_OUT_COMPLETE,
        ESE_THREAD_LOAD_COMPLETE,
    }
}
