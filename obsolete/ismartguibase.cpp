
/************************************************************************
*    FILE NAME:       ismartguibase.h
*
*    DESCRIPTION:     iSmartGuiBase Class
************************************************************************/

// Physical component dependency
#include <gui/ismartguibase.h>

// Game lib dependencies
#include <gui/icontrol.h>
#include <gui/menu.h>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CSmartGuiControl::CSmartGuiControl( iControl * piControl ) :
    m_piControl(piControl)
{
}   // constructor


/************************************************************************
*    DESC:  Constructor
************************************************************************/
CSmartGuiMenu::CSmartGuiMenu( CMenu * pUIMenu ) :
    m_pUIMenu(pUIMenu)
{
}   // constructor







