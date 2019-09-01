/************************************************************************
*    FILE NAME:       menudefs.h
*
*    DESCRIPTION:     game menu defines
************************************************************************/

#ifndef __ui_menu_defs_h__
#define __ui_menu_defs_h__

namespace NMenuDefs
{
    enum EMenuState
    {
        EMS_INACTIVE=0,
        EMS_IDLE,
        EMS_ACTIVE,
        EMS_MAX_MENU_STATES
    };

    enum EMenuNavigationState
    {
        EMNS_NULL=0,
        EMNS_MOUSE,
        EMNS_GAMEPAD_KEYBAORD,
        EMNS_MAX_MENU_NAV_STATES
    };

    enum EMenuTreeState
    {
        EMTS_INACTIVE=0,
        EMTS_IDLE,
        EMTS_ACTIVE,
        EMTS_MAX_MENU_TREE_STATES
    };

    enum EMenuEvent
    {
        // Menu events
        EME_MENU_USER_EVENTS = 0x8000,
        EME_MENU_TRANS_IN,
        EME_MENU_TRANS_OUT,
        EME_MENU_ESCAPE_ACTION,
        EME_MENU_TOGGLE_ACTION,
        EME_MENU_BACK_ACTION,
        EME_MENU_TO_TREE,
        EME_MENU_TO_MENU,
        EME_MENU_GAME_STATE_CHANGE,
        EME_MENU_UP_ACTION,
        EME_MENU_DOWN_ACTION,
        EME_MENU_LEFT_ACTION,
        EME_MENU_RIGHT_ACTION,
        EME_MENU_CONTROL_STATE_CHANGE,
        EME_MENU_SELECT_ACTION,
        EME_MENU_SELECT_EXECUTE,
        EME_MENU_SET_ACTIVE_CONTROL,
        EME_MENU_REACTIVATE,
        EME_MENU_SCROLL_UP,
        EME_MENU_SCROLL_DOWN,
        EME_MENU_SCROLL_LEFT,
        EME_MENU_SCROLL_RIGHT,
        EME_MENU_TAB_LEFT,
        EME_MENU_TAB_RIGHT,
    };

    enum EActiveControl
    {
        EAC_NULL=0,
        EAC_FIRST_ACTIVE_CONTROL,
        EAC_LAST_ACTIVE_CONTROL,
    };

    enum ETransCode
    {
        ETC_RESET=0,
        ETC_BEGIN,
        ETC_END,
    };
}

#endif
