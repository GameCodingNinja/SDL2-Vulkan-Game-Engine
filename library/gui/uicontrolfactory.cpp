/************************************************************************
*    FILE NAME:       uicontrolfactory.cpp
*
*    DESCRIPTION:     Class factory for control creation
************************************************************************/

// Physical component dependency
#include <gui/uicontrolfactory.h>

// Game lib dependencies
#include <utilities/exceptionhandling.h>
#include <utilities/xmlParser.h>
#include <gui/uilabel.h>
#include <gui/uibutton.h>
#include <gui/uisubcontrol.h>
#include <gui/uibuttonlist.h>
#include <gui/uicheckbox.h>
#include <gui/uislider.h>
#include <gui/uiscrollbox.h>
#include <gui/uimeter.h>
#include <gui/uiprogressbar.h>

// Boost lib dependencies
#include <boost/format.hpp>

namespace NUIControlFactory
{
    /************************************************************************
    *    DESC:  Load the control info from XML node
    ************************************************************************/
    iControl * Create( const XMLNode & node, const std::string & group )
    {
        iControl * pControl;

        // Get the control type. This is required
        std::string ctrlType = node.getAttribute( "controlType" );

        // New up the control with its respected control type
        if( ctrlType == "label" )
            pControl = new CUILabel( group );

        else if( ctrlType == "button" )
                pControl = new CUIButton( group );

        else if( ctrlType == "sub_control" )
                pControl = new CUISubControl( group );

        else if( ctrlType == "button_list" )
                pControl = new CUIButtonList( group );

        else if( ctrlType == "check_box" )
                pControl = new CUICheckBox( group );

        else if( ctrlType == "slider" )
                pControl = new CUISlider( group );

        else if( ctrlType == "scroll_box" )
                pControl = new CUIScrollBox( group );
        
        else if( ctrlType == "meter" )
                pControl = new CUIMeter( group );
        
        else if( ctrlType == "progress_bar" )
                pControl = new CUIProgressBar( group );

        else
            throw NExcept::CCriticalException("UI Factory Control Load Error!",
                boost::str( boost::format("UI Control not defined (%s).\n\n%s\nLine: %s")
                    % ctrlType % __FUNCTION__ % __LINE__ ));

        // Have the control load it's share
        pControl->loadFromNode( node );

        return pControl;
    }
}
