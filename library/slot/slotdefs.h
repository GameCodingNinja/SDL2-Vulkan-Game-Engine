
/************************************************************************
*    FILE NAME:       slotdefs.h
*
*    DESCRIPTION:     misc slot defines
************************************************************************/

#ifndef __slot_defs_h__
#define __slot_defs_h__

namespace NSlotDefs
{
    enum ESlotDevice
    {
        ED_NULL,
        ED_REEL,
        ED_WHEEL,
    };
    
    enum ESpinDirection
    {
        ESD_UP,
        ESD_DOWN,
        ESD_LEFT,
        ESD_RIGHT,
        EDS_CLOCKWISE,
        EDS_COUNTERCLOCKWISE,
    };
    
    enum EPayType
    {
        EP_PAYLINE,
        EP_SCATTER,
    };
    
    enum ESpinState
    {
        ESS_STOPPED,
        ESS_SPIN_STARTING,
        ESS_SPINNING,
        ESS_PREPARE_TO_STOP,
        ESS_SPIN_STOPPING
    };
    
    enum ESlotState
    {
        ESLOT_IDLE,
        ESLOT_KILL_CYCLE_RESULTS,
        ESLOT_PLACE_WAGER,
        ESLOT_GENERATE_STOPS,
        ESLOT_EVALUATE,
        ESLOT_PRE_SPIN,
        ESLOT_SPIN,
        ESLOT_POST_SPIN,
        ESLOT_PRE_AWARD_WIN,
        ESLOT_BONUS_DECISION,
        ESLOT_PRE_BONUS,
        ESLOT_BONUS,
        ESLOT_POST_BONUS,
        ESLOT_POST_AWARD_WIN,
        ESLOT_WAIT_FOR_AWARD,
        ESLOT_END
    };

}   // NSlotDefs

#endif  // __slot_defs_h__


