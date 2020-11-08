/************************************************************************
*    FILE NAME:       menudefs.as
*
*    DESCRIPTION:     Menu defines
************************************************************************/

namespace NMenuEvent
{
    shared enum EEvent
    {
        // Menu events
        USER_EVENTS = 0x8000,
        ROOT_TRANS_IN,
        TRANS_IN,
        TRANS_OUT,
        ESCAPE_ACTION,
        TOGGLE_ACTION,
        BACK_ACTION,
        TO_TREE,
        TO_MENU,
        GAME_STATE_CHANGE,
        UP_ACTION,
        DOWN_ACTION,
        LEFT_ACTION,
        RIGHT_ACTION,
        CONTROL_STATE_CHANGE,
        SELECT_ACTION,
        SELECT_EXECUTE,
        SET_ACTIVE_CONTROL,
        REACTIVATE,
        SCROLL_UP,
        SCROLL_DOWN,
        SCROLL_LEFT,
        SCROLL_RIGHT,
        TAB_LEFT,
        TAB_RIGHT,
    }
}

namespace NControlState
{
    enum EControlState
    {
        _NULL=0,
        ROOT_TRANS_IN,
        TRANS_IN,
        TRANS_OUT,
        DISABLE,
        INACTIVE,
        ACTIVE,
        SELECT,
        CHANGE,
        EXECUTE,
        EVENT
    }
}

namespace NTransCode
{
    shared enum ETransCode
    {
        BEGIN,
        END,
    }
}