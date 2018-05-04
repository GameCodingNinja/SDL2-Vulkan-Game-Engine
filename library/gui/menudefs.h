/************************************************************************
*    FILE NAME:       uimenudefs.h
*
*    DESCRIPTION:     game menu defines
************************************************************************/

#ifndef __ui_menu_defs_h__
#define __ui_menu_defs_h__

namespace NMenu
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

    enum EGameEvent
    {
        // Menu events
        EGE_MENU_USER_EVENTS = 0x8000,
        EGE_MENU_TRANS_IN,
        EGE_MENU_TRANS_OUT,
        EGE_MENU_ESCAPE_ACTION,
        EGE_MENU_TOGGLE_ACTION,
        EGE_MENU_BACK_ACTION,
        EGE_MENU_TO_TREE,
        EGE_MENU_TO_MENU,
        EGE_MENU_GAME_STATE_CHANGE,
        EGE_MENU_UP_ACTION,
        EGE_MENU_DOWN_ACTION,
        EGE_MENU_LEFT_ACTION,
        EGE_MENU_RIGHT_ACTION,
        EGE_MENU_CONTROL_STATE_CHANGE,
        EGE_MENU_SELECT_ACTION,
        EGE_MENU_SELECT_EXECUTE,
        EGE_MENU_SET_ACTIVE_CONTROL,
        EGE_MENU_REACTIVATE,
        EGE_MENU_SCROLL_UP,
        EGE_MENU_SCROLL_DOWN,
        EGE_MENU_SCROLL_LEFT,
        EGE_MENU_SCROLL_RIGHT,
        EGE_MENU_TAB_LEFT,
        EGE_MENU_TAB_RIGHT,
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

}   // NMenu

#endif  // __ui_menu_defs_h__