
/************************************************************************
*    FILE NAME:       uicheckbox.h
*
*    DESCRIPTION:     Class for user interface check box buttons
************************************************************************/

#ifndef __ui_check_box_h__
#define __ui_check_box_h__

// Physical component dependency
#include <gui/uicontrol.h>

class CUICheckBox : public CUIControl
{
public:

    // Constructor
    CUICheckBox( const std::string & group );

    // Destructor
    virtual ~CUICheckBox();

    // Set or get the Select state of this control
    void setToggleState( bool value );
    bool getToggleState();

    // Render the button
    //virtual void render( const CMatrix & matrix ) override;

protected:

    virtual void onSelectExecute( const SDL_Event & rEvent ) override;
    
private:

    // Select state
    bool m_toggleState;
};

#endif
