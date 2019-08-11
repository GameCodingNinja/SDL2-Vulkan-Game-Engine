/************************************************************************
*    FILE NAME:       uicontroldefs.h
*
*    DESCRIPTION:     game ui control defines
************************************************************************/

#ifndef __ui_control_defs_h__
#define __ui_control_defs_h__

namespace NUIControlDefs
{
    const bool TOGGLE_STATE_ON(true);
    const bool TOGGLE_STATE_OFF(false);

    // Value for no active control
    const int NO_ACTIVE_CONTROL(-1);

    // Increment/Decement constants
    enum
    {
        BTN_DEC,
        BTN_INC,
    };

    enum EControlType
    {
        ECT_NULL=0,
        ECT_LABEL,
        ECT_BUTTON,
        ECT_BUTTON_LIST,
        ECT_CHECK_BOX,
        ECT_SLIDER,
        ECT_SCROLL_BOX,
        ECT_SUB_CONTROL,
        ECT_METER,
        ECT_TAB_CONTROL,
        ECT_PROGRESS_BAR,
        ECT_AMOUNT_BUTTON
    };

    enum EControlState
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

    enum EControlActionType
    {
        ECAT_IDLE=-1,
        ECAT_NULL,
        ECAT_TO_TREE,
        ECAT_TO_MENU,
        ECAT_BACK,
        ECAT_CLOSE,
        ECAT_CHANGE_FOCUS,
        ECAT_GAME_STATE_CHANGE,
        ECAT_QUIT_GAME
    };
}

#endif
