/************************************************************************
*    FILE NAME:       statedefs.as
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
        ESE_STATE_USER_EVENTS = 0x8100,
        ESE_FADE_IN_COMPLETE,
        ESE_FADE_OUT_COMPLETE,
        ESE_THREAD_LOAD_COMPLETE
    }
}

namespace NTitleDefs
{
    enum ESpriteId
    {
        SPRITE_PEG = -2
    }
}

namespace NLevelDefs
{
    enum ELevelEvent
    {
        ELE_LEVEL_USER_EVENTS = 0x8200,
        ELE_BANG_UP_AWARD,
        ELE_RESTART_GAME,
    }

    enum ESpriteState
    {
        ESS_BANG_UP_AWARD = NObjectParams::STATE1,
    }

    enum ESpriteId
    {
        SPRITE_PEG = -2,
        SPRITE_MULTI = -3
    }
}
