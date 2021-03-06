/************************************************************************
*    FILE NAME:       menudefs.as
*
*    DESCRIPTION:     Menu defines
************************************************************************/

namespace NMenuDefs
{
    shared enum EMenuEvent
    {
        // Menu events
        EME_MENU_USER_EVENTS = 0x8000,
        EME_MENU_ROOT_TRANS_IN,
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
    }

    shared enum ETransCode
    {
        ETC_RESET=0,
        ETC_BEGIN,
        ETC_END,
    }
    
    // Button states
    shared enum EActionPress
    {
        EAP_IDLE = 0,
        EAP_DOWN,
        EAP_UP
    }
}