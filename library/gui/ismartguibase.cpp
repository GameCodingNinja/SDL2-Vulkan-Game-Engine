
/************************************************************************
*    FILE NAME:       ismartguibase.h
*
*    DESCRIPTION:     iSmartGuiBase Class
************************************************************************/

// Physical component dependency
#include <gui/ismartguibase.h>

// Game lib dependencies
#include <gui/uicontrol.h>
#include <gui/menu.h>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CSmartGuiControl::CSmartGuiControl( CUIControl * pUIControl ) :
    m_pUIControl(pUIControl)
{
}   // constructor


/************************************************************************
*    DESC:  Constructor
************************************************************************/
CSmartGuiMenu::CSmartGuiMenu( CMenu * pUIMenu ) :
    m_pUIMenu(pUIMenu)
{
}   // constructor







