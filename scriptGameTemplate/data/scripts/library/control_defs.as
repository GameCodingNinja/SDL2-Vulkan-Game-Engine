/************************************************************************
*    FILE NAME:       menu_defs.as
*
*    DESCRIPTION:     Menu defines
************************************************************************/

namespace NControlDefs
{
    shared enum EControlState
    {
        ECS_NULL=0,
        ECS_INIT,
        ECS_TRANS_IN,
        ECS_TRANS_OUT,
        ECS_DISABLE,
        ECS_INACTIVE,
        ECS_ACTIVE,
        ECS_SELECT,
        ECS_CHANGE,
        ECS_EXECUTE,
        ECS_EVENT,
    };
}