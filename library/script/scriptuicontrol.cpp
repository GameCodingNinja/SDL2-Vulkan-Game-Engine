
/************************************************************************
*    FILE NAME:       scriptuicontrol.cpp
*
*    DESCRIPTION:     uiControl Interface script object registration
************************************************************************/

// Physical component dependency
#include <script/scriptuicontrol.h>

// Game lib dependencies
#include <gui/uiscrollbox.h>
#include <gui/icontrol.h>
#include <script/scriptmanager.h>
#include <script/scriptglobals.h>
#include <utilities/exceptionhandling.h>
#include <utilities/genfunc.h>

// Boost lib dependencies
#include <boost/format.hpp>

// AngelScript lib dependencies
#include <angelscript.h>
#include <scriptarray/scriptarray.h>

namespace NScriptUIControl
{
    CPoint<float> point;
    
    /************************************************************************
    *    DESC:  Get Resolutions
    ************************************************************************/
    CScriptArray * getScrollboxControls(iControl & control)
    {
        // Make sure this control is a scroll box
        if( control.getType() != NUIControl::ECT_SCROLL_BOX )
            throw NExcept::CCriticalException("Scroll box array of controls error!",
                boost::str( boost::format("Control is not a scroll box.\n\n%s\nLine: %s")
                    % __FUNCTION__ % __LINE__ ));
        
        auto & scrollCtrlVec = NGenFunc::DynCast<CUIScrollBox>(&control)->getScrollCtrlVec();
        
        // Get a pointer to type declaration
        asITypeInfo * arrayType = CScriptMgr::Instance().getPtrToTypeInfo( "array<uiControl>" );
        
        CScriptArray* ary = CScriptArray::Create(arrayType, scrollCtrlVec.size());
        for( size_t i = 0; i < scrollCtrlVec.size(); ++i )
            ary->SetValue(i, scrollCtrlVec[i]);
        
        return ary;
    }

    /************************************************************************
    *    DESC:  Wrapper function due to virtual inheritance
    ************************************************************************/
    void SetAlpha(float value, iControl & control)
    {
        control.setAlpha(value);
    }

    float GetAlpha(iControl & control)
    {
        return control.getAlpha();
    }

    void SetVisible(bool value, iControl & control)
    {
        control.setVisible(value);
    }

    bool IsVisible(iControl & control)
    {
        return control.isVisible();
    }

    void SetPos1(const CPoint<float> & pos, iControl & control)
    {
        control.setPos(pos);
    }

    void SetPos2(float x, float y, float z, iControl & control)
    {
        control.setPos(x,y,z);
    }

    void IncPos1(const CPoint<float> & pos, iControl & control)
    {
        control.incPos(pos);
    }

    void IncPos2(float x, float y, float z, iControl & control)
    {
        control.incPos(x,y,z);
    }

    const CPoint<float> & GetPos(iControl & control)
    {
        point = control.getPos();
        return point;
    }

    void SetRot1(const CPoint<float> & rot, bool convertToRadians, iControl & control)
    {
        control.setRot(rot, convertToRadians);
    }

    void SetRot2(float x, float y, float z, bool convertToRadians, iControl & control)
    {
        control.setRot(x,y,z, convertToRadians);
    }

    void IncRot1(const CPoint<float> & rot, bool convertToRadians, iControl & control)
    {
        control.incRot(rot, convertToRadians);
    }

    void IncRot2(float x, float y, float z, bool convertToRadians, iControl & control)
    {
        control.incRot(x,y,z, convertToRadians);
    }

    const CPoint<float> & GetRot(iControl & control)
    {
        return control.getRot();
    }

    void SetScale1(const CPoint<float> & rot, iControl & control)
    {
        control.setScale(rot);
    }

    void SetScale2(float x, float y, float z, iControl & control)
    {
        control.setScale(x,y,z);
    }

    void IncScale1(const CPoint<float> & rot, iControl & control)
    {
        control.incScale(rot);
    }

    void IncScale2(float x, float y, float z, iControl & control)
    {
        control.incScale(x,y,z);
    }

    const CPoint<float> & GetScale(iControl & control)
    {
        return control.getScale();
    }

    const std::string & GetName(iControl & control)
    {
        return control.getName();
    }

    const std::string & GetGroup(iControl & control)
    {
        return control.getGroup();
    }

    const std::string & GetFaction(iControl & control)
    {
        return control.getFaction();
    }

    /************************************************************************
    *    DESC:  Register the class with AngelScript
    ************************************************************************/
    void Register()
    {
        using namespace NScriptGlobals; // Used for Throw

        asIScriptEngine * pEngine = CScriptMgr::Instance().getEnginePtr();

        // Register type
        Throw( pEngine->RegisterObjectType(  "uiControl", 0, asOBJ_REF|asOBJ_NOCOUNT) );

        Throw( pEngine->RegisterObjectMethod("uiControl", "void setStringToList(string &in)",   asMETHOD(iControl, setStringToList), asCALL_THISCALL) );
        
        Throw( pEngine->RegisterObjectMethod("uiControl", "void createFontString(string &in, int spriteIndex = 0)",          asMETHODPR(iControl, createFontString, (const std::string &, int), void), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("uiControl", "void createFontString(int stringIndex = 0, int spriteIndex = 0)", asMETHODPR(iControl, createFontString, (int, int), void), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("uiControl", "void setActionType(string &in)",      asMETHODPR(iControl, setActionType, (const std::string &), void), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("uiControl", "void setExecutionAction(string &in)", asMETHOD(iControl, setExecutionAction),   asCALL_THISCALL) );
        
        Throw( pEngine->RegisterObjectMethod("uiControl", "bool isDisabled()",                   asMETHOD(iControl, isDisabled),   asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("uiControl", "bool isInactive()",                   asMETHOD(iControl, isInactive),   asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("uiControl", "bool isActive()",                     asMETHOD(iControl, isActive),     asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("uiControl", "bool isSelected()",                   asMETHOD(iControl, isSelected),   asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("uiControl", "bool isSelectable()",                 asMETHOD(iControl, isSelectable), asCALL_THISCALL) );

        Throw( pEngine->RegisterObjectMethod("uiControl", "const string & getName()",            asFUNCTION(GetName), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("uiControl", "const string & getGroup()",           asFUNCTION(GetGroup), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("uiControl", "const string & getFaction()",         asFUNCTION(GetFaction), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("uiControl", "const string & getExecutionAction()", asMETHOD(iControl, getExecutionAction), asCALL_THISCALL) );
        
        // Object pos, rot & scale
        Throw( pEngine->RegisterObjectMethod("uiControl", "void setVisible(bool)",                                 asFUNCTION(SetVisible), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("uiControl", "bool isVisible()",                                      asFUNCTION(IsVisible), asCALL_CDECL_OBJLAST) );

        Throw( pEngine->RegisterObjectMethod("uiControl", "void setPos(CPoint &in)",                               asFUNCTION(SetPos1), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("uiControl", "void setPos(float x = 0, float y = 0, float z = 0)",    asFUNCTION(SetPos2), asCALL_CDECL_OBJLAST) );

        Throw( pEngine->RegisterObjectMethod("uiControl", "void incPos(CPoint & in)",                              asFUNCTION(IncPos1), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("uiControl", "void incPos(float x = 0, float y = 0, float z = 0)",    asFUNCTION(IncPos2), asCALL_CDECL_OBJLAST) );

        Throw( pEngine->RegisterObjectMethod("uiControl", "const CPoint & getPos()",                               asFUNCTION(GetPos),  asCALL_CDECL_OBJLAST) );

        Throw( pEngine->RegisterObjectMethod("uiControl", "void setRot(CPoint &in, bool convertToRadians = true)", asFUNCTION(SetRot1), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("uiControl", "void setRot(float x = 0, float y = 0, float z = 0, bool convertToRadians = true)", asFUNCTION(SetRot2), asCALL_CDECL_OBJLAST) );

        Throw( pEngine->RegisterObjectMethod("uiControl", "void incRot(CPoint &in, bool convertToRadians = true)", asFUNCTION(IncRot1), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("uiControl", "void incRot(float x = 0, float y = 0, float z = 0, bool convertToRadians = true)", asFUNCTION(IncRot2), asCALL_CDECL_OBJLAST) );

        Throw( pEngine->RegisterObjectMethod("uiControl", "const CPoint & getRot()",                               asFUNCTION(GetRot),    asCALL_CDECL_OBJLAST) );

        Throw( pEngine->RegisterObjectMethod("uiControl", "void setScale(CPoint & in)",                            asFUNCTION(SetScale1), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("uiControl", "void setScale(float x = 1, float y = 1, float z = 1)",  asFUNCTION(SetScale2), asCALL_CDECL_OBJLAST) );

        Throw( pEngine->RegisterObjectMethod("uiControl", "void incScale(CPoint & in)",                            asFUNCTION(IncScale1), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("uiControl", "void incScale(float x = 1, float y = 1, float z = 1)",  asFUNCTION(IncScale2), asCALL_CDECL_OBJLAST) );

        Throw( pEngine->RegisterObjectMethod("uiControl", "const CPoint & getScale()",                             asFUNCTION(GetScale),   asCALL_CDECL_OBJLAST) );
        
        // Interface to button list
        Throw( pEngine->RegisterObjectMethod("uiControl", "int getActiveIndex()",                                  asMETHOD(iControl, getActiveIndex), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("uiControl", "void setActiveIndex(int)",                              asMETHOD(iControl, setActiveIndex), asCALL_THISCALL) );
        
        // Interface to check box
        Throw( pEngine->RegisterObjectMethod("uiControl", "bool getToggleState()",                                 asMETHOD(iControl, getToggleState), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("uiControl", "void setToggleState(bool)",                             asMETHOD(iControl, setToggleState), asCALL_THISCALL) );
        
        // Interface to meter
        Throw( pEngine->RegisterObjectMethod("uiControl", "void setMeterValue(double)",                            asMETHOD(iControl, setMeterValue), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("uiControl", "void startBangUp(double)",                              asMETHOD(iControl, startBangUp), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("uiControl", "void fastBang()",                                       asMETHOD(iControl, fastBang), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("uiControl", "bool isBanging()",                                      asMETHOD(iControl, isBanging), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("uiControl", "void clearMeter()",                                     asMETHOD(iControl, clearMeter), asCALL_THISCALL) );
        
        // Interface to progress bar
        Throw( pEngine->RegisterObjectMethod("uiControl", "void setProgressBarValue(float)",                       asMETHOD(iControl, setProgressBarValue), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("uiControl", "float getProgressBarValue()",                           asMETHOD(iControl, getProgressBarValue), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("uiControl", "void incProgressBarValue(float)",                       asMETHOD(iControl, incProgressBarValue), asCALL_THISCALL) );
        
        Throw( pEngine->RegisterObjectMethod("uiControl", "void setProgressBarMinValue(float)",                    asMETHOD(iControl, setProgressBarMinValue), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("uiControl", "float getProgressBarMinValue()",                        asMETHOD(iControl, getProgressBarMinValue), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("uiControl", "void setProgressBarMaxValue(float)",                    asMETHOD(iControl, setProgressBarMaxValue), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("uiControl", "float getProgressBarMaxValue()",                        asMETHOD(iControl, getProgressBarMaxValue), asCALL_THISCALL) );
        
        // Interface to slider
        Throw( pEngine->RegisterObjectMethod("uiControl", "void setSliderPosValue(float)",                         asMETHOD(iControl, setSliderPosValue), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("uiControl", "float setSliderPosValue()",                             asMETHOD(iControl, setSliderPosValue), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("uiControl", "void setSliderMinValue(float)",                         asMETHOD(iControl, setSliderMinValue), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("uiControl", "void setSliderMaxValue(float)",                         asMETHOD(iControl, setSliderMaxValue), asCALL_THISCALL) );
        
        // Interface to scroll box
        Throw( pEngine->RegisterObjectMethod("uiControl", "array<CSize> @ getScrollboxControls()",                 asFUNCTION(getScrollboxControls), asCALL_CDECL_OBJLAST) );
    }
}
