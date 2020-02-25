/************************************************************************
*    FILE NAME:       leveldefs.as
*
*    DESCRIPTION:     Level defines
************************************************************************/

namespace NLevelDefs
{
    shared enum ELevelEvent
    {
        ELE_LEVEL_USER_EVENTS = 0x8200,
        ELE_BANG_UP_AWARD,
    }

    shared enum ESpriteState
    {
        ESS_BANG_UP_AWARD = NDefs::OBJ_STATE1,
    }
}
