
/************************************************************************
*    FILE NAME:       ismartguibase.h
*
*    DESCRIPTION:     Smart Gui interface & Base Classes
************************************************************************/

#ifndef __smart_gui_base_h__
#define __smart_gui_base_h__

// Standard lib dependencies
#include <vector>
#include <string>

// Forward declaration(s)
class CUIControl;
class CMenu;
union SDL_Event;

/*************************************
*  Smart GUI interface class
**************************************/
class iSmartGui
{
public:

    // Called when the control is created
    virtual void create(){}

    // Called during the handle user imput
    virtual void handleEvent( const SDL_Event & rEvent ){}
};


/*************************************
*  Smart GUI Menu class
**************************************/
class CSmartGuiMenu : public iSmartGui
{
public:

    // Constructor
    CSmartGuiMenu( CMenu * pUIMenu );

protected:

    // Pointer to UI Menu class.
    // NOTE: We do not own this pointer. Don't try to free it!
    CMenu * m_pUIMenu;
};


/*************************************
*  Smart GUI Control
**************************************/
class CSmartGuiControl : public iSmartGui
{
public:

    // Constructor
    CSmartGuiControl( CUIControl * pUIControl );

    // Called when the control is executed
    virtual void execute(){}

protected:

    // Pointer to UI Control class.
    // NOTE: We do not own this pointer. Don't try to free it!
    CUIControl * m_pUIControl;
};

#endif
