/************************************************************************
*    FILE NAME:       uidefs.h
*
*    DESCRIPTION:     game ui defines
************************************************************************/

#pragma once

namespace NUIDefs
{
    enum EUIDefs
    {
        NO_ACTIVE_CONTROL = -1
    };
}

namespace NActiveControl
{
    enum EActiveControl
    {
        FIRST,
        LAST
    };
}

namespace NTransCode
{
    enum ETransCode
    {
        BEGIN,
        END
    };
}

enum class EAction
{
    DEC   = 0,
    INC   = 1,

    UP    = 1,
    DOWN  = 2,
    LEFT  = 4,
    RIGHT = 8,

    ON = true,
    OFF = false
};

enum class EControlType
{
    _NULL,
    LABEL,
    BUTTON,
    BUTTON_LIST,
    CHECK_BOX,
    SLIDER,
    SCROLL_BOX,
    SUB_CONTROL,
    METER,
    TAB_CONTROL,
    PROGRESS_BAR,
    AMOUNT_BUTTON
};

enum class EControlState
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
};

enum class EControlActionType
{
    IDLE=-1,
    _NULL,
    TO_TREE,
    TO_MENU,
    BACK,
    CLOSE,
    CHANGE_FOCUS,
    GAME_STATE_CHANGE,
    QUIT_GAME
};

enum class EMenuState
{
    INACTIVE,
    IDLE,
    ACTIVE
};

namespace NMenuEvent
{
    enum EEvent
    {
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
    };
}
