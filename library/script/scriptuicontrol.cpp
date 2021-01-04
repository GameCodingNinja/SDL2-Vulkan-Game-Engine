
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
#include <autowrapper/aswrappedcall.h>

namespace NScriptUIControl
{
    CPoint<float> point;

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

    iControl * FindControl(const std::string & name, iControl & control)
    {
        return control.findControl( name );
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

        Throw( pEngine->RegisterObjectMethod("uiControl", "void setStringToList(string &in)",                                WRAP_MFN(iControl, setStringToList), asCALL_GENERIC) );
        
        Throw( pEngine->RegisterObjectMethod("uiControl", "void createFontString(string &in, int spriteIndex = 0)",          WRAP_MFN_PR(iControl, createFontString, (const std::string &, int), void), asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("uiControl", "void createFontString(int stringIndex = 0, int spriteIndex = 0)", WRAP_MFN_PR(iControl, createFontString, (int, int), void),                 asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("uiControl", "void setActionType(string &in)",                                  WRAP_MFN_PR(iControl, setActionType, (const std::string &), void),         asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("uiControl", "void setExecutionAction(string &in)",                             WRAP_MFN(iControl, setExecutionAction),                                    asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("uiControl", "void changeState(int)",                                           WRAP_MFN(iControl, changeState),                                           asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("uiControl", "uiControl & findControl(string &in)",                             WRAP_OBJ_LAST(FindControl),                                                asCALL_GENERIC) );
        
        Throw( pEngine->RegisterObjectMethod("uiControl", "bool isDisabled() const",                               WRAP_MFN(iControl, isDisabled),                 asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("uiControl", "bool isInactive() const",                               WRAP_MFN(iControl, isInactive),                 asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("uiControl", "bool isActive() const",                                 WRAP_MFN(iControl, isActive),                   asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("uiControl", "bool isSelected() const",                               WRAP_MFN(iControl, isSelected),                 asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("uiControl", "bool isSelectable() const",                             WRAP_MFN(iControl, isSelectable),               asCALL_GENERIC) );

        Throw( pEngine->RegisterObjectMethod("uiControl", "const string & getName() const",                        WRAP_OBJ_LAST(GetName),                         asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("uiControl", "const string & getGroup() const",                       WRAP_OBJ_LAST(GetGroup),                        asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("uiControl", "const string & getFaction() const",                     WRAP_OBJ_LAST(GetFaction),                      asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("uiControl", "const string & getExecutionAction() const",             WRAP_MFN(iControl, getExecutionAction),         asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("uiControl", "bool prepare(string &in, bool f = false)",              WRAP_MFN(iControl, prepare),                    asCALL_GENERIC) );

        Throw( pEngine->RegisterObjectMethod("uiControl", "void removeScriptFunction(string &in)",                 WRAP_MFN(iControl, removeScriptFunction),        asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("uiControl", "void addScriptFunction(string &in, string &in, string &in, bool f1 = false, bool f2 = false, bool f3 = false)", WRAP_MFN(iControl, addScriptFunction), asCALL_GENERIC) );
        
        // Object pos, rot & scale
        Throw( pEngine->RegisterObjectMethod("uiControl", "void setVisible(bool)",                                 WRAP_OBJ_LAST(SetVisible),                         asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("uiControl", "bool isVisible() const",                                WRAP_OBJ_LAST(IsVisible),                          asCALL_GENERIC) );

        Throw( pEngine->RegisterObjectMethod("uiControl", "void setPos(CPoint &in)",                               WRAP_OBJ_LAST(SetPos1),                            asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("uiControl", "void setPos(float x = 0, float y = 0, float z = 0)",    WRAP_OBJ_LAST(SetPos2),                            asCALL_GENERIC) );

        Throw( pEngine->RegisterObjectMethod("uiControl", "void incPos(CPoint & in)",                              WRAP_OBJ_LAST(IncPos1),                            asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("uiControl", "void incPos(float x = 0, float y = 0, float z = 0)",    WRAP_OBJ_LAST(IncPos2),                            asCALL_GENERIC) );

        Throw( pEngine->RegisterObjectMethod("uiControl", "const CPoint & getPos() const",                         WRAP_OBJ_LAST(GetPos),                             asCALL_GENERIC) );

        Throw( pEngine->RegisterObjectMethod("uiControl", "void setRot(CPoint &in, bool convertToRadians = true)", WRAP_OBJ_LAST(SetRot1),                            asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("uiControl", "void setRot(float x = 0, float y = 0, float z = 0, bool convertToRadians = true)", WRAP_OBJ_LAST(SetRot2), asCALL_GENERIC) );

        Throw( pEngine->RegisterObjectMethod("uiControl", "void incRot(CPoint &in, bool convertToRadians = true)", WRAP_OBJ_LAST(IncRot1),                            asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("uiControl", "void incRot(float x = 0, float y = 0, float z = 0, bool convertToRadians = true)", WRAP_OBJ_LAST(IncRot2), asCALL_GENERIC) );

        Throw( pEngine->RegisterObjectMethod("uiControl", "const CPoint & getRot() const",                         WRAP_OBJ_LAST(GetRot),                             asCALL_GENERIC) );

        Throw( pEngine->RegisterObjectMethod("uiControl", "void setScale(CPoint & in)",                            WRAP_OBJ_LAST(SetScale1),                          asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("uiControl", "void setScale(float x = 1, float y = 1, float z = 1)",  WRAP_OBJ_LAST(SetScale2),                          asCALL_GENERIC) );

        Throw( pEngine->RegisterObjectMethod("uiControl", "void incScale(CPoint & in)",                            WRAP_OBJ_LAST(IncScale1),                          asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("uiControl", "void incScale(float x = 1, float y = 1, float z = 1)",  WRAP_OBJ_LAST(IncScale2),                          asCALL_GENERIC) );

        Throw( pEngine->RegisterObjectMethod("uiControl", "const CPoint & getScale() const",                       WRAP_OBJ_LAST(GetScale),                           asCALL_GENERIC) );

        // Interface to sub sontrol
        Throw( pEngine->RegisterObjectMethod("uiControl", "uint size()",                                           WRAP_MFN(iControl, size),                       asCALL_GENERIC) );
        
        // Interface to button list
        Throw( pEngine->RegisterObjectMethod("uiControl", "int getActiveIndex() const",                            WRAP_MFN(iControl, getActiveIndex),             asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("uiControl", "void setActiveIndex(int)",                              WRAP_MFN(iControl, setActiveIndex),             asCALL_GENERIC) );
        
        // Interface to check box
        Throw( pEngine->RegisterObjectMethod("uiControl", "bool getToggleState() const",                           WRAP_MFN(iControl, getToggleState),             asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("uiControl", "void setToggleState(bool)",                             WRAP_MFN(iControl, setToggleState),             asCALL_GENERIC) );
        
        // Interface to meter
        Throw( pEngine->RegisterObjectMethod("uiControl", "void setMeterValue(double)",                            WRAP_MFN(iControl, setMeterValue),              asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("uiControl", "void startBangUp(double)",                              WRAP_MFN(iControl, startBangUp),                asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("uiControl", "void incBangUp(double)",                                WRAP_MFN(iControl, incBangUp),                  asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("uiControl", "void fastBang()",                                       WRAP_MFN(iControl, fastBang),                   asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("uiControl", "bool isBanging() const",                                WRAP_MFN(iControl, isBanging),                  asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("uiControl", "void clearMeter()",                                     WRAP_MFN(iControl, clearMeter),                 asCALL_GENERIC) );
        
        // Interface to progress bar
        Throw( pEngine->RegisterObjectMethod("uiControl", "void setProgressBarValue(float)",                       WRAP_MFN(iControl, setProgressBarValue),        asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("uiControl", "float getProgressBarValue() const",                     WRAP_MFN(iControl, getProgressBarValue),        asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("uiControl", "void incProgressBarValue(float)",                       WRAP_MFN(iControl, incProgressBarValue),        asCALL_GENERIC) );
        
        Throw( pEngine->RegisterObjectMethod("uiControl", "void setProgressBarMinValue(float)",                    WRAP_MFN(iControl, setProgressBarMinValue),     asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("uiControl", "float getProgressBarMinValue() const",                  WRAP_MFN(iControl, getProgressBarMinValue),     asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("uiControl", "void setProgressBarMaxValue(float)",                    WRAP_MFN(iControl, setProgressBarMaxValue),     asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("uiControl", "float getProgressBarMaxValue() const",                  WRAP_MFN(iControl, getProgressBarMaxValue),     asCALL_GENERIC) );
        
        // Interface to slider
        Throw( pEngine->RegisterObjectMethod("uiControl", "void setSliderPosValue(float)",                         WRAP_MFN(iControl, setSliderPosValue),          asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("uiControl", "float getSliderPosValue() const",                       WRAP_MFN(iControl, getSliderPosValue),          asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("uiControl", "void setSliderMinValue(float)",                         WRAP_MFN(iControl, setSliderMinValue),          asCALL_GENERIC) );
        Throw( pEngine->RegisterObjectMethod("uiControl", "void setSliderMaxValue(float)",                         WRAP_MFN(iControl, setSliderMaxValue),          asCALL_GENERIC) );
        
        // Interface to scroll box
        Throw( pEngine->RegisterObjectMethod("uiControl", "uiControl & getScrollBoxControl(uint)",                 WRAP_MFN(iControl, getPtrToScrollBoxControl),   asCALL_GENERIC) );
    }
}
