/************************************************************************
*    FILE NAME:       uicontroldefs.as
*
*    DESCRIPTION:     game ui control defines
************************************************************************/

namespace NUIControlDefs
{
    shared enum EControlState
    {
        ECS_NULL=0,
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